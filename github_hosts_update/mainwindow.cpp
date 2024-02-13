#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0, 100); // 设置进度条范围
    manager=new QNetworkAccessManager(this);
    QNetworkRequest request(my_url);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
        if (bytesTotal > 0) {
            int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
            ui->progressBar->setValue(progress);
        }
    });
    connect(reply, &QNetworkReply::finished, [this,reply]() {
        // 下载完成后进行处理
        qDebug()<<"finished!";
        QByteArray data = reply->readAll(); // 获取响应的数据
        if(data.isEmpty()==false){
            QString filePath = QCoreApplication::applicationDirPath()   + "/new.zip"; // 构建文件路径
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(data); // 将数据写入文件
                file.close();
            } else {
                qWarning() << "无法写入文件" << filePath;
            }
            reply->deleteLater();
            QProcess process;
            QString command = "7z x new.zip -y";
            QProcess::startDetached(command);
            this->close();


        }else{
            qDebug()<<"the file is empty!!!";
        }

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
