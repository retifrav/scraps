#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // устанавливаем в текст надписи начальное значение "часов"
    ui->label->setText(QDateTime::currentDateTime()
                       // это формат вывода значения даты и времени в строку
                       .toString("yyyy-MM-dd hh:mm:ss"));

    // создаём таймер
    QTimer *timer = new QTimer(this);
    // соединяем его сигнал истечения времени с нашим слотом действия
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    // запускаем таймер и указываем периодичность сигналирования (1 секунда)
    timer->start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime()
{
    // устанавливаем в текст надписи текущее значение "часов"
    ui->label->setText(QDateTime::currentDateTime()
                       // это формат вывода значения даты и времени в строку
                       .toString("yyyy-MM-dd hh:mm:ss"));
}
