#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 读取设置
    loadSettings();

    // 设置检测网络连接定时器
    connect(&m_networkCheckTimer, &QTimer::timeout, this, &MainWindow::checkNetworkConnection);
    m_networkCheckTimer.start(3600000); // 启动后和每小时检测一次

    // 设置自动更新检测定时器
    connect(&m_updateCheckTimer, &QTimer::timeout, this, &MainWindow::checkForUpdates);
    m_updateCheckTimer.start(10800000); // 启动后和每三个小时检测一次
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings()
{
    // 检查是否存在setting.ini文件，若不存在则创建
    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/setting.ini";
    if (!QDir().exists(settingsPath)) {
        QSettings settings(settingsPath, QSettings::IniFormat);
        settings.setValue("Auto/start", 0);
    }

    // 读取设置
    m_settings = QSettings(settingsPath, QSettings::IniFormat);
    int autoStart = m_settings.value("Auto/start", 0).toInt();
    ui->checkBox->setChecked(autoStart == 1);
}

void MainWindow::saveSettings()
{
    // 更新设置
    int autoStart = ui->checkBox->isChecked() ? 1 : 0;
    m_settings.setValue("Auto/start", autoStart);
    m_settings.sync();

    // 启用/禁用开机自启
    QString appPath = QCoreApplication::applicationFilePath();
    QSettings runSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (autoStart == 1) {
        runSettings.setValue("MyApp", appPath);
    } else {
        runSettings.remove("MyApp");
    }
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    saveSettings();
}

void MainWindow::checkNetworkConnection()
{
    // TODO: 根据需要实现网络连接检测逻辑，在检测到连接异常时进行处理
    // 若连接异常，可以使用以下代码弹窗提示用户并终止自身：
    // QMessageBox::critical(this, "错误", "网络连接异常，请检查网络设置。");
    // QCoreApplication::quit();
}

void MainWindow::checkForUpdates()
{
    if (m_updating) {
        // 正在进行更新操作，则不进行检测
        return;
    }

    // 调用API接口检查是否有更新
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://api.baiwulin.com/app"));
    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        QString version = jsonObj.value("version").toString();
        QString log = jsonObj.value("log").toString();
        bool isForceUpdate = jsonObj.value("force").toBool();

        // 判断是否需要进行更新
        QString currentVersion = "1.0.0";  // 当前程序版本
        if (version > currentVersion) {
            // 弹窗提示更新
            QMessageBox::StandardButton reply;
            if (isForceUpdate) {
                reply = QMessageBox::critical(this, "发现新版本", "有新版本可用，请立即更新。\n" + log, QMessageBox::Ok);
            } else {
                reply = QMessageBox::question(this, "发现新版本", "有新版本可用，是否更新？\n" + log, QMessageBox::Yes | QMessageBox::No);
            }

            if (reply == QMessageBox::Yes) {
                m_updating = true;

                QString url = jsonObj.value("url").toString();
                QString updateType = jsonObj.value("type").toString();
                if (updateType == "web") {
                    // 在浏览器中打开更新链接
                    QDesktopServices::openUrl(QUrl(url));
                } else if (updateType == "auto") {
                    // 自动下载更新文件
                    QNetworkAccessManager downloadManager;
                    QNetworkRequest downloadRequest(QUrl(url));
                    QNetworkReply* downloadReply = downloadManager.get(downloadRequest);
                    QEventLoop downloadLoop;
                    connect(downloadReply, &QNetworkReply::finished, &downloadLoop, &QEventLoop::quit);
                    downloadLoop.exec();

                    if (downloadReply->error() == QNetworkReply::NoError) {
                        QString filePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/update.exe";
                        QFile file(filePath);
                        if (file.open(QIODevice::WriteOnly)) {
                            file.write(downloadReply->readAll());
                            file.close();
                        }

                        // 启动更新程序
                        QProcess::startDetached(filePath);

                        // 退出当前程序
                        QCoreApplication::quit();
                    }
                }
            }
        }
    }
}
