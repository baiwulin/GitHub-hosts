#include "form_update.h"
#include "ui_form_update.h"

Form_update::Form_update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Form_update)
{
    qDebug()<<"start update downloading";
    ui->setupUi(this);
    manager=new QNetworkAccessManager(this);
    manager_download=new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://cs.baiwulin.com/app/githubHosts/update"));
    QNetworkReply *reply = manager->get(request);
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
                    QNetworkRequest request(QUrl("https://cs.baiwulin.com/app/githubHosts/update"));
                    QNetworkReply *reply_download = manager_download->get(request);
                    connect(reply_download, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
                        if (bytesTotal > 0) {
                            int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
                            ui->progressBar->setValue(progress);
                        }
                    });
                    connect(reply_download, &QNetworkReply::finished, [reply_download,this]() {
                        QByteArray data = reply_download->readAll(); // 获取响应的数据
                        if(data.isEmpty()==false){
                            QDir().mkpath("c:\\gitub_host_update");
                            QFile file("c:\\gitub_host_update\\up.exe");
                            if (file.open(QIODevice::WriteOnly)) {
                                file.write(data); // 将数据写入文件
                                file.close();
                                QProcess process;
                                QString command = "c:\\gitub_host_update\\up.exe -silent";
                                QProcess::startDetached(command);
                                this->close();
                            } else {
                                qWarning() << "无法写入文件" ;
                            }
                            reply_download->deleteLater();
                    }


                });
                }else{
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

Form_update::~Form_update()
{
    delete ui;

}
