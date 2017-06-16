#include "serverStuff.h"

ServerStuff::ServerStuff(QObject* pwgt)
    : QObject(pwgt), m_nNextBlockSize(0)
{
    theServer = new QTcpServer(this);
}

void ServerStuff::slotNewConnection()
{
    QTcpSocket* pClientSocket = theServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()));
    connect(pClientSocket, SIGNAL(disconnected()),
            this, SLOT(gotDisconnection()));

    sendToClient(pClientSocket, "connection established");
}

void ServerStuff::slotReadClient()
{
    pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
                if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QString str;
        in >> str;

        // QString strMessage = "Client has sended - " + str;

        emit gotSmth(str);

        m_nNextBlockSize = 0;

        sendToClient(pClientSocket,
                     "Server Response: Received \"" + str + "\""
                    );
    }
}

void ServerStuff::gotDisconnection()
{
    emit smbDisconnected();
}

void ServerStuff::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}
