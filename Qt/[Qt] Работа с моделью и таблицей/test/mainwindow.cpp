#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // установка текста для надписи (в дизайнере не удалось использовать теги)
    ui->label->setText(tr("Заполните поля (можно не все) и нажмите "
                          "<b>Добавить строку</b>"));

    // создаём модель на 4 столбца и иначально без строк
    model = new QStandardItemModel(0, 4, this);
    // задаём заголовки столбцов
    model->setHeaderData(0, Qt::Horizontal, tr("Первый столбец"));
    model->setHeaderData(1, Qt::Horizontal, tr("Второй столбец"));
    model->setHeaderData(2, Qt::Horizontal, tr("Третий столбец"));
    model->setHeaderData(3, Qt::Horizontal, tr("Четвёртый столбец"));

    // присваиваем модель таблице
    ui->tableView->setModel(model);
    // устанавливаем автоматическое изменение ширины столбцов под размеры окна
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // создаём элементы модели (ячейки). Их четыре - по числу столбцов
    QStandardItem *item1, *item2, *item3, *item4;
    // это просто временная строка для сохранения значений полей ввода
    QString lineText;

    // получаем текст из поля ввода
    lineText = ui->lineEdit_1->text();
    // если там есть текст, то создаём элемент с таким текстом
    if(!lineText.isEmpty()) item1 = new QStandardItem(lineText);
    // а если поле ввода пустое, то создаём элемент без текста
    else item1 = new QStandardItem();

    lineText = ui->lineEdit_2->text();
    if(!lineText.isEmpty()) item2 = new QStandardItem(lineText);
    else item2 = new QStandardItem();

    lineText = ui->lineEdit_3->text();
    if(!lineText.isEmpty()) item3 = new QStandardItem(lineText);
    else item3 = new QStandardItem();

    lineText = ui->lineEdit_4->text();
    if(!lineText.isEmpty()) item4 = new QStandardItem(lineText);
    else item4 = new QStandardItem();

    // создаём список элементов (ячеек). Это будет одна строка
    QList<QStandardItem *> itemlist;
    // заполняем этот список (строку) элементами (ячейками)
    itemlist << item1 << item2 << item3 << item4;
    // добавляем этот список, как строку, в нашу модель
    model->appendRow(itemlist); /* изменения в модели отобразятся
    и в таблице, но важно понимать, что мы добавили строку не в таблицу,
    а именно в модель */

    // изменяем текст надписи, отображающей количество строк
    ui->row_value->setText(QString::number(model->rowCount()));
}
