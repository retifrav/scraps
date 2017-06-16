#include "clientStuff.h"

ClientStuff::ClientStuff(
        const QString host,
        int port,
        QObject* parent
        ) : QObject(parent), m_nNextBlockSize(0)
{
    _status = false;
    theSocket = new QTcpSocket(this);

    _host = host;
    _port = port;

    connect(theSocket, SIGNAL(disconnected()),
            this, SLOT(closeConnection()));
    connect(theSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(theSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

void ClientStuff::connect2host()
{
    theSocket->connectToHost(_host, _port);
}

void ClientStuff::slotConnected()
{
    _status = true;
    emit statusChanged(_status);
}

bool ClientStuff::get_status() {return _status;}

void ClientStuff::slotReadyRead()
{
    QDataStream in(theSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (theSocket->bytesAvailable() < sizeof(quint16)) { break; }
            in >> m_nNextBlockSize;
        }

        if (theSocket->bytesAvailable() < m_nNextBlockSize) { break; }

        QTime time;
        QString str;
        in >> time >> str;
        if (str == "0")
        {
            closeConnection();
            return;
        }
        str = time.toString() + " " + str;
        emit hasReadSome(str);
        m_nNextBlockSize = 0;
    }
}

void ClientStuff::gotDisconnection()
{
    _status = false;
    emit statusChanged(_status);
}

void ClientStuff::closeConnection()
{
    theSocket->close();
    gotDisconnection();
}
