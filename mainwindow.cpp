#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkAccessManager>//网络模块
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>//josn处理
#include <QJsonObject>

#include <QTimer>
#include <QDebug>
#include<qdebug.h>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon = QIcon(":/new/prefix1/icon/logo.ico");
    settings = new QSettings("setting.ini",QSettings::IniFormat);
    if(settings->value("main/checkbox").toString()=="true"){
        ui->checkBox->setChecked(true);
    }
    if(settings->value("main/checkbox1").toString()=="true"){
        ui->checkBox_2->setChecked(true);
    }
    my_trayIcon = new QSystemTrayIcon(this);
    trayMenu = new QMenu(this);
    my_trayIcon->setIcon(icon);   
    // 添加操作到菜单
    QAction *action1 = trayMenu->addAction("显示/隐藏");
     connect(action1, &QAction::triggered, this, &MainWindow::minimizeWindow);
    QAction *action2 = trayMenu->addAction("退出");
     connect(action2, &QAction::triggered, this, &MainWindow::closeWindow);
    // 将菜单设置为托盘图标的上下文菜单
    my_trayIcon->setContextMenu(trayMenu);
    connect(my_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    my_trayIcon->show();
    //配置SSL
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    QSslConfiguration::setDefaultConfiguration(sslConfig);
    if (!QSslSocket::supportsSsl()) {
        QMessageBox::information(NULL, "warning", "notSupportsSsl", QMessageBox::Yes);
    }
    //版本检测
    manager_version=new QNetworkAccessManager(this);
    manager_update=new QNetworkAccessManager(this);
    manager_host=new QNetworkAccessManager(this);

    //删除更新残留文件
    QDir dir("c:\\gitub_host_update");
    if (dir.exists()) {
            // 删除文件夹及其内容
            if (dir.removeRecursively()) {
                qDebug() << "Folder removed successfully.";
            } else {
                qDebug() << "Failed to remove folder.";
            }
        } else {
            qDebug() << "Folder does not exist.";
        }

    timer_version = new QTimer(this);    
    connect( timer_version, &QTimer::timeout, this, &MainWindow::fetchversion);
    timer_version->start(3 * 3600 * 1000);
    MainWindow::fetchversion();
    timer_host=new QTimer(this);
    connect( timer_version, &QTimer::timeout, this, &MainWindow::changehost);
    timer_host->start(600 * 1000);
    MainWindow::changehost();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete my_trayIcon;
}


void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (this->isVisible()) {
            this->hide();
        } else {
            this->show();
        }
    }
}

void MainWindow::minimizeWindow()
{
    if (this->isVisible()) {
        this->hide();
    } else {
        this->show();
    }
}

void MainWindow::closeWindow()
{
    this->close();
}

//状态栏函数
void MainWindow::statusBar(int color, QString data)
{
    QPixmap color_0(":/new/prefix1/icon/red.ico");
    QPixmap color_1(":/new/prefix1/icon/blue.ico");
    QPixmap color_2(":/new/prefix1/icon/green.ico");
    switch(color){
    case 0:
        ui->label_2->setPixmap(color_0);
        break;
    case 1:
        ui->label_2->setPixmap(color_1);
        break;
    case 2:
        ui->label_2->setPixmap(color_2);
        qDebug()<<"color2";
        break;
    }
    ui->label_5->setText(data);
}

//开机自启复选框槽函数
void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        settings->setValue("main/checkbox","true");
        setAutoStart(true);
        QMessageBox::information(nullptr, "Information", "设置成功");
    }
    else{
        settings->setValue("main/checkbox","false");
        setAutoStart(false);
        QMessageBox::information(nullptr, "Information", "取消成功");
    }

}

//是否启用复选框槽函数
void MainWindow::on_checkBox_2_clicked()
{
    QString myhost=settings->value("main/host").toString().replace("[charge]"," \n");
    QFile file_host(QString("C:/Windows/System32/drivers/etc/hosts"));
    file_host.open(QIODevice::ReadOnly | QIODevice::Text);
    QString systemhost=QTextStream(&file_host).readAll();
    file_host.close();
    int startIndex = systemhost.indexOf("# GitHub520 Host Start");
    int endIndex = systemhost.indexOf("# GitHub520 Host End");
    if(ui->checkBox_2->isChecked()){
        settings->setValue("main/checkbox1","true");
        if(startIndex == -1 || endIndex == -1){
            systemhost.append(myhost);
        }
    }else{
        settings->setValue("main/checkbox1","false");
        if (startIndex != -1 || endIndex != -1){
            settings->setValue("main/host",systemhost.mid(startIndex, endIndex - startIndex + 22).replace("\n","[charge]"));
            systemhost.remove(startIndex, endIndex - startIndex + 22);
            statusBar(0,QString("加速已禁用！！！"));
        }
    }
    file_host.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file_host);
    out << systemhost.toStdString().c_str();
    file_host.close();
    MainWindow::changehost();
}


//开机自启函数
void MainWindow::setAutoStart(bool enable)
{
    QString applicationName = QCoreApplication::applicationName();
    QString applicationPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    QString registryPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    QSettings settings(registryPath, QSettings::NativeFormat);

    if (enable) {
        settings.setValue(applicationName, applicationPath);
    } else {
        settings.remove(applicationName);
    }
}

void MainWindow::fetchversion()
{
    QNetworkRequest request(QUrl("https://cs.baiwulin.com/app/githubHosts/version"));
    QNetworkReply *reply = manager_version->get(request);
    connect(reply, &QNetworkReply::finished, [reply,this]() {
        // 处理响应
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            if (!jsonDoc.isNull()) {
                QJsonObject jsonObject = jsonDoc.object();
                int version = jsonObject["version"].toString().toInt();
                QString log = jsonObject["log"].toString().replace("[change]", "\n");
                QString realname = jsonObject["name"].toString();
                bool force = jsonObject["force"].toBool();
                qDebug() << "Version:" << version;
                 qDebug() << "\nrealVersion:" << realname;
                qDebug()<<"\nlog:\n"<<log;
                qDebug()<<"\nforce:\n"<<force;

                if(version>2){
                    if(force!=true){
                        this->show();
                        this->hide();
                        QMessageBox::StandardButton rb;
                        rb = QMessageBox::question(this, "是否要更新", "有更新,版本:"+realname+"\n日志:\n"+log);
                        qDebug() << "newversion:" << version;
                        if (rb == QMessageBox::Yes) {
                            // "是"
                            this->show();
                            Form_update fu;
                            fu.setWindowTitle("更新中...");
                            fu.exec();
                            this->close();
                        }else{
                            this->show();
                        }

                    }else{
                        this->show();
                        this->hide();
                        QMessageBox::StandardButton rb;
                        rb = QMessageBox::question(this, "是否要更新", " 您的版本过于老旧,请使用最新版本\n版本:"+realname+"\n日志:\n"+log);
                        qDebug() << "newversion:" << version;
                        if (rb == QMessageBox::Yes) {
                            // "是"
                             this->show();
                            Form_update fu;
                            fu.setWindowTitle("更新中...");
                            fu.exec();
                            this->close();
                        }else{
                             this->show();
                             this->close();
                        }

                    }

                }

            }
        }else{
            qDebug() << "Error fetching data:" << reply->errorString();
        }
        reply->deleteLater();
    });

}

//更新调用函数
void MainWindow::fetchupdate(){
    QNetworkRequest request(QUrl("https://cs.baiwulin.com/app/githubHosts/update"));
    QNetworkReply *reply = manager_version->get(request);
    connect(reply, &QNetworkReply::finished, [reply,this]() {
        // 处理响应
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            if (!jsonDoc.isNull()) {
                QJsonObject jsonObject = jsonDoc.object();
                QString type= jsonObject["type"].toString();
                QString url= jsonObject["url"].toString();
                qDebug() << "type:" <<type;
                qDebug()<<"\nurl:"<<url;
                if(type=="auto"){
                    QByteArray byteArray = url.toUtf8();
                    QByteArray urlEncoded = byteArray.toBase64();
                    QProcess process;
                    QString command = QCoreApplication::applicationDirPath()+"/github_hosts_update.exe "+QString::fromUtf8(urlEncoded);
                    QProcess::startDetached(command);
                    this->close();
                }
                else{
                    QDesktopServices::openUrl(url);
                    this->close();
                }
            }
        }else{
            qDebug() << "Error fetching data:" << reply->errorString();
        }
        reply->deleteLater();
    });

}

//host更新函数
void MainWindow::changehost(){
    if(ui->checkBox_2->isChecked()){
        QFile file_host(QString("C:/Windows/System32/drivers/etc/hosts"));
        file_host.open(QIODevice::ReadOnly | QIODevice::Text);
        QString systemhost=QTextStream(&file_host).readAll();
        QNetworkRequest request(QUrl("https://cs.baiwulin.com/app/githubHosts/hosts"));
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);   //使能重定向
        QNetworkReply *reply = manager_host->get(request);
        connect(reply, &QNetworkReply::finished, [reply,this,systemhost]()mutable {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                if(responseData.isEmpty()==false){
                    QString newhost = QString::fromUtf8(responseData);
                    QString fileName = "host.bak";
                    QString baseName = fileName;
                    int counter = 1;
                    while (QFile::exists(QCoreApplication::applicationDirPath()+"/"+fileName)) {
                           fileName = QString("%1%2.bak").arg(baseName.left(baseName.lastIndexOf("."))).arg(counter++);
                       }
                    QFile file(QCoreApplication::applicationDirPath()+"/"+fileName);
                    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                           QTextStream stream(&file);
                           stream << systemhost;
                           file.close();
                           qDebug() << "backup host to file:" << fileName;

                           int startIndex = systemhost.indexOf("# GitHub520 Host Start");
                           int endIndex = systemhost.indexOf("# GitHub520 Host End");
                           if (startIndex == -1 || endIndex == -1){
                           systemhost.append("\n").append(newhost);
                           }else{
                               systemhost.replace(startIndex, endIndex - startIndex + 22, newhost);
                           }
                           QFile file_host(QString("C:/Windows/System32/drivers/etc/hosts"));
                           if (file_host.open(QIODevice::WriteOnly | QIODevice::Text)){
                               QTextStream streamh(&file_host);
                               streamh << systemhost;
                               file_host.close();
                               statusBar(2,QString("host 已更改..."));
                               qDebug() << "sucess to change host" ;
                           }else{
                               qDebug() << "fail to change host" ;
                               statusBar(0,QString("无权限更改host或更改错误！！！"));
                           }

                       } else {
                           qDebug() << "Failed to backup host to file:" << fileName;
                           statusBar(0,QString("备份host失败，请检查是否有权限！！！"));
                       }
                    file.close();
                    reply->deleteLater();
                }else{
                    statusBar(0,QString("未知的错误！！！"));
                }
            }else{
                qDebug()<<"get host fail";
                statusBar(0,QString("链接host源失败！！！"));

            }

        });
        file_host.close();
    }
}

void MainWindow::on_pushButton_sure_clicked()
{
    this->hide();
}

void MainWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) {
            // 处理点击关闭按钮的情况
            hide();
            event->ignore();
        } else {
            // 处理其他方式关闭窗口的情况
            event->accept();
        }
}
