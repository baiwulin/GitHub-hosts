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
    if(settings.value("main/checkbox").toString()=="true"){

     }else{
         w.show();
     }
    return a.exec();
}

