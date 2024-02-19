#ifndef FORM_UPDATE_H
#define FORM_UPDATE_H

#include <QDialog>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSettings>

#include <QVariant>//转换qt变量

#include <QStandardPaths>//取路径及设置注册表等
#include <QDesktopServices>
#include <QCoreApplication>
#include <QDir>
#include <Windows.h>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QProcess>
#include <QByteArray>

#include <QNetworkAccessManager>//网络模块
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>//josn处理
#include <QJsonObject>

#include <QTimer>
#include <QDebug>
#include<qdebug.h>

namespace Ui {
class Form_update;
}

class Form_update : public QDialog
{
    Q_OBJECT

public:
    explicit Form_update(QWidget *parent = 0);
    ~Form_update();

private:
    Ui::Form_update *ui;
    QNetworkAccessManager *manager;
    QNetworkAccessManager *manager_download;
};

#endif // FORM_UPDATE_H
