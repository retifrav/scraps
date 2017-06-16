#include <QtGui>
#include <QTextCodec>
#include <QWaitCondition>
#include "mainwindow.h"

void sleep(int ms)
{
 QWaitCondition sleep;
 QMutex mutex;
 sleep.wait(&mutex, ms);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QSplashScreen *splash = new QSplashScreen(QPixmap(":/pics/splash.jpg"));
    splash->showMessage(QString::fromUtf8("<font size=5 color=white><b>v0.9.1.867</b></font><br><font size=3 color=white>2012</font>"), Qt::AlignRight);
    splash->show();

    sleep(5000);

    MainWindow w;
    w.show();

    splash->finish(&w);
    delete splash;

    return a.exec();
}
