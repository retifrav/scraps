#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // создадим объект стандартной модели на 4 строки и 3 столбца
    _model = new QStandardItemModel(4, 3, this);

    // создаём объект нашей прокси
    Model4table *proxah = new Model4table(this);
    // модель теперь смотрится через проксю
    proxah->setModel(_model);

    // тут присваиваем ячейкам значения
    QModelIndex index = _model->index(0, 2, QModelIndex());
    // в одну пишем 1
    _model->setData(index, 1, Qt::DisplayRole);
    // в другую пишем ololo
    _model->setData(index.sibling(1, 1), "ololo", Qt::DisplayRole);

    // устанавливаем для таблицы нашу проксю в качестве модели
    ui->tableView->setModel(proxah);

    // можно видеть, как различаются данные в проксе и в реальной модели
    qDebug() << ui->tableView->model()->data(index);
    qDebug() << _model->data(index);
}

MainWindow::~MainWindow()
{
    delete ui;
}
