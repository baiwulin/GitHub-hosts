#include "mainwindow.h"
#include <QApplication>
QUrl my_url;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QByteArray data=QByteArray::fromBase64(argv[1]);
    my_url=QString::fromUtf8(data);
    MainWindow w;
    w.setWindowTitle("更新中...");
    w.show();
    return a.exec();
}
