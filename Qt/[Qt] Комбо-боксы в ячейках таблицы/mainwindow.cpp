#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // устанавливаем автоматическое изменение ширины столбцов под размеры окна
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    // например, нам надо вставить КомбоБоксы только во 2 столбец
    for(int x = 0; x < ui->tableWidget->rowCount(); x++)
        {
            for(int y = 0; y < ui->tableWidget->columnCount(); y++)
            {
                // создаём очередной КомбоБокс
                QComboBox *cmb = new QComboBox();
                // засовываем в него поля
                cmb->addItem(""); // сначала пустое, но можно и без пустого
                cmb->addItem(tr("первое поле"));
                cmb->addItem(tr("второе что-нибудь"));
                cmb->addItem(tr("третье ещё"));
                cmb->setCurrentIndex(3);
                // если 2 столбец, то помещаем КомбоБокс в ячейку
                if(y == 1) ui->tableWidget->setCellWidget(x, y, cmb);
            }
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    for(int x = 0; x < ui->tableWidget->rowCount(); x++)
    {
        QComboBox *curCmb =
                static_cast<QComboBox*>(ui->tableWidget->cellWidget(x, 1));
        curCmb->setCurrentIndex(ui->spinBox->value());
    }
}
