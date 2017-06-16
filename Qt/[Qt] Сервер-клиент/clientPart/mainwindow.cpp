#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_disconnect->setVisible(false);

    _client = new ClientStuff("localhost", 6547);

    setStatus(_client->get_status());

    connect(_client, SIGNAL(hasReadSome(QString)),
            this, SLOT(write2log(QString)));
    connect(_client, SIGNAL(statusChanged(bool)),
            this, SLOT(setStatus(bool)));
    connect(_client->theSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(gotError(QAbstractSocket::SocketError)));
//    connect(_client, SIGNAL(record2log(QString)),
//            this, SLOT(write2log(QString)));
}

MainWindow::~MainWindow()
{
    delete _client;
    delete ui;
}

void MainWindow::setStatus(bool newStatus)
{
    if(newStatus)
    {
        ui->label_status->setText(
                    tr("<font color=\"green\">Соединение установлено</font>"));
        ui->pushButton_connect->setVisible(false);
        ui->pushButton_disconnect->setVisible(true);
    }
    else
    {
        ui->label_status->setText(
                tr("<font color=\"red\">Соединение разорвано</font>"));
        ui->pushButton_connect->setVisible(true);
        ui->pushButton_disconnect->setVisible(false);
    }
}

void MainWindow::write2log(QString record)
{
    ui->textEdit_log->append(record);
}

void MainWindow::gotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(_client->theSocket->errorString())
                    );
    ui->textEdit_log->append(strError);
}

void MainWindow::on_pushButton_connect_clicked()
{
    _client->connect2host();
}

void MainWindow::on_pushButton_send_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << ui->lineEdit_message->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    _client->theSocket->write(arrBlock);
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    _client->closeConnection();
}
