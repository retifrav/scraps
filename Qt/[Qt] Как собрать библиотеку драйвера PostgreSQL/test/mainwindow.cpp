#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlDatabase>

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
    QStringList list = QSqlDatabase::drivers();
    for(int x = 0; x < list.length(); x++)
        ui->log->append(list.at(x));

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    ui->log->append(tr("---<br><b>QPSQL-driver is ")
                    +(db.isValid()
                      ? tr("<font color=green>OK</font></b>")
                        : tr("<font color=red>NOT</font></b>")));
}
