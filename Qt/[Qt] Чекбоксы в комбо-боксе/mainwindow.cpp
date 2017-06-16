#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // подключаемся к БД
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("12321");
    if(!db.open())
    {
        QMessageBox::critical(
                    this,
                    tr("Ошибка подключения к БД"),
                    tr("Не удалось подключиться к БД.\n")
                        + db.lastError().text()
                    );
        return;
    }
    else QMessageBox::information(
                this,
                tr("Всё огонь"),
                tr("Успешное подключение к БД.")
                );

    // создаём модель запроса и выполняем его в ней
    QSqlQueryModel *model = new QSqlQueryModel;
    // запрашиваем имена полей таблицы
    model->setQuery("SELECT column_name FROM information_schema.columns "
                    "WHERE information_schema.columns.table_name = "
                    "'ВАША_ТАБЛИЦА';");

    // создаём модель для комбо-бокса по числу строк из модели запроса
    QStandardItemModel *model4combo =
            new QStandardItemModel(model->rowCount(), 1, this);

    // переносим данные из модели запроса в модель для комбо-бокса
    QModelIndex index; // индекс для навигации по модели запроса
    for (int i = 0; i < model4combo->rowCount(); i++)
    {
        index = model->index(i, 0, QModelIndex());
        // создаём элемент для добавления в модель комбо-бокса
        QStandardItem* item = new QStandardItem(model->data(index).toString());
        // расставляем флаги, чтобы каждый элемент модели бы чекуемым
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Checked, Qt::CheckStateRole);
        // добавляем элемент в модель для комбо-бокса
        model4combo->setItem(i, 0, item);
    }

    // назначаем комбо-боксу нашу модель
    ui->comboBox->setModel(model4combo);
}

MainWindow::~MainWindow()
{
    delete ui;
}
