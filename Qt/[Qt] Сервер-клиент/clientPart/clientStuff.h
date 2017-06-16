#ifndef CLIENTSTUFF_H
#define CLIENTSTUFF_H

#include <QString>
#include <QTcpSocket>
#include <QTime>

class ClientStuff : public QObject
{
    Q_OBJECT

public:
    ClientStuff(const QString host, int port, QObject *parent = 0);

    QTcpSocket* theSocket;
    bool get_status();

public slots:
//    void connect2server(QString host, quint16 port);
//    void gotConnection();
//    void sendRequest(const QString msg);
//    void getAnswer();
//    void errorOccured();
//    void connectionClosedByServer();
    void gotDisconnection();
    void closeConnection();
    void connect2host();

signals:
    void statusChanged(bool);
    void hasReadSome(QString msg);
    //    void record2log(QString);

private slots:
    void slotReadyRead();
    void slotConnected();

private:
    QString _host;
    int _port;
    bool _status;
    quint16 m_nNextBlockSize;
};

#endif // CLIENTSTUFF_H
