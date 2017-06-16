#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _server = new ServerStuff(this);

    connect(_server, SIGNAL(gotSmth(QString)),
            this, SLOT(gotSmth(QString)));
    connect(_server->theServer, SIGNAL(newConnection()),
            this, SLOT(smbConnectedToServer()));
    connect(_server, SIGNAL(smbDisconnected()),
            this, SLOT(smbDisconnectedFromServer()));
}

MainWindow::~MainWindow()
{
    delete _server;
    delete ui;
}

void MainWindow::on_pushButton_startServer_clicked()
{
    if (!_server->theServer->listen(QHostAddress::Any, 6547))
    {

        ui->textEdit_log->append(tr("<font color=\"red\"><b>Ошибка открытия порта!</b> Порт занят.</font>"));
        return;
    }
    connect(_server->theServer, SIGNAL(newConnection()),
            _server, SLOT(slotNewConnection()));
    ui->textEdit_log->append(tr("<font color=\"green\"><b>Порт открыт</b> и успешно занят для прослушивания.</font>"));
}

void MainWindow::on_pushButton_stopServer_clicked()
{
    if(_server->theServer->isListening())
    {
        disconnect(_server->theServer, SIGNAL(newConnection()),
                   _server, SLOT(slotNewConnection()));
        for(int i = 0; i < _server->theServer->children().count(); i++)
        {
            _server->pClientSocket = (QTcpSocket*)_server->theServer->children().at(i);
            _server->sendToClient(_server->pClientSocket, "<font color=\"red\">connection closed</font>");
            _server->sendToClient(_server->pClientSocket, "0");
        }
        _server->theServer->close();
        ui->textEdit_log->append(tr("<b>Порт закрыт.</b>"));

    }
    else
    {
        ui->textEdit_log->append(tr("<b>Порт и не был открыт, нечего закрывать.</b>"));
    }
}

void MainWindow::on_pushButton_testConn_clicked()
{
    if(_server->theServer->isListening())
    {
        ui->textEdit_log->append(tr("Сервер работает. Всего подключений: ")
                                 + QString::number(
                                     _server->theServer->children().count() - 1));
    }
    else
    {
        ui->textEdit_log->append(tr("Сервер не работает. Всего подключений: ")
                                 + QString::number(
                                     _server->theServer->children().count()));
    }
}

void MainWindow::smbConnectedToServer()
{
    ui->textEdit_log->append(tr("Кто-то подключился!"));
}

void MainWindow::smbDisconnectedFromServer()
{
    ui->textEdit_log->append(tr("А теперь кто-то отключился..."));
}

void MainWindow::gotSmth(QString msg)
{
    ui->textEdit_log->append(msg);
}
