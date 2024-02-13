#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void minimizeWindow();
    void closeWindow();
    void on_checkBox_clicked();
    void fetchversion();
    void fetchupdate();


private:
    Ui::MainWindow *ui;
    void setAutoStart(bool enable);
    QSystemTrayIcon *my_trayIcon;
    QMenu *trayMenu;
    QSettings *settings;
    QTimer *timer_version;
    QNetworkAccessManager *manager_version;
    QNetworkAccessManager *manager_update;
};

#endif // MAINWINDOW_H
