#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon icon = QIcon(":/new/prefix1/icon/图标.ico");
    w.setWindowIcon(icon);
    w.setWindowTitle("GitHub_hosts");
    w.show();
    return a.exec();
}

