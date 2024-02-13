#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QNetworkAccessManager>//网络模块
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QProcess>
#include <QByteArray>

#include <QTimer>
#include <QMessageBox>

extern QUrl my_url;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
};

#endif // MAINWINDOW_H
