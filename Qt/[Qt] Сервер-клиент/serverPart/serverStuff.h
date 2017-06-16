#ifndef SERVERSTUFF_H
#define SERVERSTUFF_H

#include "QTcpServer"
#include "QTcpSocket"
#include "QMessageBox"
#include <QTime>

class ServerStuff : public QObject
{
    Q_OBJECT

public:
    ServerStuff(QObject *pwgt);
    QTcpServer* theServer;
    QTcpSocket* pClientSocket;

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void gotDisconnection();
    void sendToClient(QTcpSocket* pSocket, const QString& str);

signals:
    void gotSmth(QString msg);
    void smbDisconnected();

private:
    quint16 m_nNextBlockSize;
};

#endif // SERVERSTUFF_H
