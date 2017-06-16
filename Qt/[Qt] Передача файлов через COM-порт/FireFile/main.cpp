#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.setWindowTitle(QString::fromUtf8("FireFile - передача файлов"));
    w.show();

    return a.exec();
}
