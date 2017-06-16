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

void MainWindow::on_toolButton_getSchemaName_clicked()
{
    schemaFileName = QFileDialog::getOpenFileName
            (
                this,
                tr("Выберите файл схемы"),
                QString(),
                tr("XSD (*.xsd)")
            );
    ui->lineEdit_schemaName->setText(schemaFileName);
}

void MainWindow::on_toolButton_getFileName_clicked()
{
    importFileName = QFileDialog::getOpenFileName
            (
                this,
                tr("Выберите файл для импорта"),
                QString(),
                tr("XML (*.xml)")
            );
    ui->lineEdit_fileName->setText(importFileName);
}

void MainWindow::on_pushButton_check_clicked()
{
    if(ui->lineEdit_schemaName->text().isEmpty()
            || ui->lineEdit_fileName->text().isEmpty())
    {
        QMessageBox::warning(this,
                              tr("Не выбраны файлы"),
                              tr("Вы должны выбрать оба файла.")
                              );
        return;
    }

    //открытие файла со схемой
    QFile schemaFile(schemaFileName);
    if(!schemaFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,
                              tr("Ошибка открытия файла схемы"),
                              tr("Не удалось открыть файл схемы.\n%1").
                              arg(schemaFile.errorString())
                              );
        return;
    }

    // открытие XML-файла, который будем проверять
    QFile importFile(importFileName);
    if (!importFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,
                              tr("Ошибка открытия XML-файла"),
                              tr("Не удалось открыть указанный файл.\n%1").
                              arg(importFile.errorString())
                              );
        return;
    }

    // создание объекта схемы и загрузка в него прочитанной из файла
    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));
    schemaFile.close();

    // проверка самой схемы
    if(!schema.isValid())
    {
        QMessageBox::critical(this,
                              tr("Ошибка схемы"),
                              tr("XML-схема не прошла проверку.")
                              );
        return;
    }
    else
    {
        QXmlSchemaValidator validator(schema);
        if(!validator.validate(&importFile, QUrl::fromLocalFile(importFile.fileName())))
        {
            QMessageBox::critical(this,
                                  tr("Несоответствие схеме"),
                                  tr("Файл не прошёл проверку по XML-схеме.")
                                  );
            return;
        }
        else
        {
            QMessageBox::information(this,
                                  tr("Ну всё огонь"),
                                  tr("Файл прошёл проверку по XML-схеме.")
                                  );
        }
    }
    importFile.close();
}
