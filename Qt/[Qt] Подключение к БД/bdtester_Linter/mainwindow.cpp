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
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Linter 6.0");
    db.setUserName("admin");
    db.setPassword("1");
    bool isConcted = db.open();
    if(isConcted) {
                   ui->statusBar->showMessage(tr("Подлючение к БД выполнено успешно"), 5000);
                   ui->pushButton->setEnabled(false);
                  }
    else {
          ui->statusBar->showMessage(tr("Подлючение не удалось"), 5000);
          ui->textEdit->append(db.lastError().text());
         }
}
