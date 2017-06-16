#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QThread>
#include <QTime>
#include <QSettings>
#include <windows.h>
#include "about.h"

namespace Ui {class MainWindow;}

class QKeyEvent;

class DemonPorta: public QThread
{
    Q_OBJECT

public:
    void run();
    void ListenPort(HANDLE ListenPort);
    HANDLE portHandle;

public slots:
    void setPortHandle(HANDLE curPort);

signals:
    void wannaUseLog(QString);
    void setProgr(int);
    void progrFin();

private:
    QSettings *configDemona;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_selectFile_clicked();
    void on_tryToConnect_clicked();
    void on_tryToDisconnect_clicked();
    void BaudRateChanged(QString param);
    void DataBitsChanged(QString param);
    void ParityChanged(int param);
    void StopBitsChanged(int param);
    void on_sendFile_clicked();
    void on_listenBtn_clicked();
    void toLog(QString strToLog);
    void setProgr(int val);
    void finProgr();

signals:
    void curPortHandle(HANDLE);

private:
    Ui::MainWindow *ui;
    QFileDialog file;
    About *ab;
    DemonPorta *ComListener;
    QSettings *config;
    QString logstr, portstr;
    QTime currtime;
    void string2log (QString logstr);
    bool ComHandShake(HANDLE UsePort);
    bool SendFile(QString FileName);
    void disableSettingsChange();
    void enableSettingsChange();
    HANDLE OpenPort(QString PortName);
    HANDLE workPort;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
