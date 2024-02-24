#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    QIcon icon = QIcon(":/new/prefix1/icon/logo.ico");
    w.setWindowIcon(icon);
    w.setWindowTitle("GitHub_hosts");
    QSettings settings("setting.ini", QSettings::IniFormat);
    if( QSettings("HKEY_CURRENT_USER\\Software\\github_hosts", QSettings::NativeFormat).value("checkbox").toString()=="true"){

    }else{
        w.show();
    }
    return a.exec();
}

