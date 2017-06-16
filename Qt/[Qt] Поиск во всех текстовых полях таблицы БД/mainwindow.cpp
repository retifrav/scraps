#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _model = new QSqlQueryModel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_search_clicked()
{
    // строка запроса
    QString query = "SELECT column_name "
            "FROM information_schema.columns "
            "WHERE table_name = '"
            + ui->lineEdit_table->text() +
            "' AND table_schema = '"
            + ui->lineEdit_schema->text() +
            "' AND (data_type = 'character' "
            "OR data_type = 'character varying' "
            "OR data_type = 'text');";

    // создание процесса на выполнение запроса
    DbProcess *getTextFields = new DbProcess(
                dbptSqlCommand,
                "Запрос имён текстовых полей",
                query
                );

    // запуск процесса
    getTextFields->startDbProcess();

    // проверка выполнения запроса
    if(!getTextFields->getSqlResultIsActive())
    {
        QMessageBox::warning(
                    this,
                    tr("Ошибка запроса"),
                    tr("Не удалось выполнить запрос"),
                    QMessageBox::Ok
                    );
        return;
    }

    // индекс нужен для навигации по модели результатов запроса
    QModelIndex index;
    // модель для сохранения результатов запроса имён текстовых полей
    QSqlQueryModel *tempModel = getTextFields->getSqlResultToModel();
    delete getTextFields; // больше оно нам не нужно
    // сюда сложим полученные имена текстовых полей
    QStringList textFields;

    // возьмём из каждой строки модели (столбец у нас один) имя текстового поля
    for(int i = 0; i < tempModel->rowCount(); i++)
    {
        index = tempModel->index(i, 0, QModelIndex());
        textFields << tempModel->data(index).toString();
    }

    // можно посмотреть, какие поля в запрашиваемой таблице оказались текстовыми
    qDebug() << textFields;

    // --- теперь начинаем искать "по шаблону" ---------------------------------

    // суть такова: составить запрос с LIKE для каждого из найденных
    // текстовых полей

    // возьмём ключевое слово
    QString keyword = ui->lineEdit_keyword->text();

    // строка запроса
    query = "SELECT * FROM " + ui->lineEdit_schema->text()
            + "." + ui->lineEdit_table->text() + " WHERE ";
    // заполняем запрос именами полей из нашего списка
    for(int i = 0; i < textFields.count(); i++)
    {
        query += textFields.at(i) + " LIKE '%" + keyword + "%' OR ";
    }

    // отрезаем последний OR и ставим ;
    query.chop(4); query += ";";

    // создание процесса на выполнение запроса
    DbProcess *templateSearch = new DbProcess(
                dbptSqlCommand,
                "Поиск по шаблону, что бы это ни значило",
                query
                );

    // запуск процесса
    templateSearch->startDbProcess();

    // проверка выполнения запроса
    if(!templateSearch->getSqlResultIsActive())
    {
        QMessageBox::warning(
                    this,
                    tr("Ошибка запроса"),
                    tr("Не удалось выполнить запрос"),
                    QMessageBox::Ok
                    );
        return;
    }

    // сохраним модель из результата запроса в нашу модель
    _model = templateSearch->getSqlResultToModel();
    delete templateSearch; // больше оно нам не нужно

    // присваиваем нашу модель нашей таблице
    ui->tableView->setModel(_model);
}
