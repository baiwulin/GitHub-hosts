#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_checkBox_clicked(bool checked);
    void checkNetworkConnection();
    void checkForUpdates();

private:
    Ui::MainWindow *ui;
    QSettings m_settings;
    QTimer m_networkCheckTimer;
    QTimer m_updateCheckTimer;
    bool m_updating;

    void loadSettings();
    void saveSettings();

    // 禁用拷贝赋值运算符
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
};

#endif // MAINWINDOW_H
