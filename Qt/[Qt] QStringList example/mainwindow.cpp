#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list = (QStringList() << "111" << "222"<< "333");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->log->append(list.join("-"));
}
void MainWindow::on_pushButton_2_clicked()
{
    list << ui->lineEdit->text();
}
