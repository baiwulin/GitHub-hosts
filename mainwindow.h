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
#include <QJsonArray>

#include <QTimer>
#include <QDebug>
#include<qdebug.h>

#include<form_update.h>//引用第二界面

#include <QLabel>
#include <QPixmap>//调整标签颜色

#include <QCloseEvent>

#include <QTime>
#include <QThread>//多线程操作。。。。

#include <QTextCodec>

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
    void refreshhost();
    void statusBar(int color,QString data);
    void on_checkBox_clicked();
    void fetchversion();
    void fetchupdate();
    void changehost();


    void on_pushButton_sure_clicked();

    void on_pushButton_cancel_clicked();

    void on_checkBox_2_clicked();

    void closeEvent(QCloseEvent *event);

    void on_pushButton_clicked();

    void myMsleep(int msec);

private:
    Ui::MainWindow *ui;
    void setAutoStart(bool enable);
    void setAutoStart2(bool enable);
    QSystemTrayIcon *my_trayIcon;
    QMenu *trayMenu;
    QSettings *settings;
    QTimer *timer_version;
    QTimer *timer_host;
    QNetworkAccessManager *manager_version;
    QNetworkAccessManager *manager_update;
    QNetworkAccessManager *manager_host;
};

class AutoStartThread : public QThread
{
public:
    AutoStartThread(bool enable) : m_enable(enable) {}

protected:
    void run() override;
private:
    bool m_enable;
};

#endif // MAINWINDOW_H
