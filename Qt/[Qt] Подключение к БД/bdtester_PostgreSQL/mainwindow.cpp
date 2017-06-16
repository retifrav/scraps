#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("PostgreSQL35W");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("1");
    bool isCnctd = db.open();
    if(!isCnctd){
        ui->textEdit->append(tr("<font color=red><b>Не удалось подключиться</b></font>"));
        qDebug() << db.lastError();
    }
    else ui->textEdit->append(tr("<font color=green><b>Успешное подключение</b></font>"));
}
