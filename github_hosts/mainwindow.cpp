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
    QIcon icon = QIcon(":/new/prefix1/icon/图标.ico");
    settings = new QSettings("setting.ini",QSettings::IniFormat);
    if(settings->value("main/checkbox").toString()=="true"){
        ui->checkBox->setChecked(true);
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
    timer_version = new QTimer(this);
    connect( timer_version, &QTimer::timeout, this, &MainWindow::fetchversion);
    timer_version->start(3 * 3600 * 1000);
    MainWindow::fetchversion();
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

void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        settings->setValue("main/checkbox","true");
        setAutoStart(true);
        QMessageBox::information(nullptr, "Information", "设置成功");
    }
    else{
        settings->setValue("main/checkbox","false");
        setAutoStart(true);
        QMessageBox::information(nullptr, "Information", "取消成功");
    }

}
//复选框槽函数

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
//开机自启函数

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
                QString version = jsonObject["version"].toString();
                QString log = jsonObject["log"].toString().replace("[change]", "\n");;
                QString force = jsonObject["force"].toString();
                qDebug() << "Version:" << version;
                qDebug()<<"\nlog:\n"<<log;
                qDebug()<<"\nforce:\n"<<force;
                if(version!="1.0.0"){
                    if(force!="ture"){
                        QMessageBox::StandardButton rb;
                        rb = QMessageBox::question(this, "是否要更新", "有更新\n版本:"+version+"\n日志:\n"+log);
                        qDebug() << "newversion:" << version;
                        if (rb == QMessageBox::Yes) {
                            // "是"
                            MainWindow::fetchupdate();
                        }


                    }else{
                        QMessageBox::StandardButton rb;
                        rb = QMessageBox::question(this, "是否要更新", "有更新\n版本:"+version+"\n日志:\n"+log);
                        qDebug() << "newversion:" << version;
                        if (rb == QMessageBox::Yes) {
                            // "是"
                            MainWindow::fetchupdate();
                        }else{
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
