#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.setWindowTitle(QString::fromUtf8("Тестер БД"));
    w.show();
    
    return a.exec();
}
