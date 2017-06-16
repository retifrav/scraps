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

void MainWindow::on_toolButton_clicked()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(
                              this,
                              tr("Выберите файл для чтения"),
                              QString(),
                              tr("TXT (*.txt)")
                              )
                          );
}

void MainWindow::on_pushButton_readFile_clicked()
{
    QFile incFile(ui->lineEdit->text());
    if (!incFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(
                    this,
                    tr("Ошибка открытия файла"),
                    tr("Не удалось открыть файл.\n%1").
                    arg(incFile.errorString())
                    );
        return;
    }

    QString s = QString();

    while (!incFile.atEnd())
    {
        s = incFile.readLine();
        ui->textEdit->append(s.simplified());
    }
}
