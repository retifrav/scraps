#include <QtGui/QApplication>
#include "mainwindow.h"
#include "initialization.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /// иницилизация по умолчанию
    Initialization *init =  new Initialization();
    /// установка стиля, кодировок для строк и названия приложения
    init->doInitialization(&a, "TestTemplateSearch");

    /// открытие соединения с БД
    DbProcess *firstProc = new DbProcess(dbptConnect, "Подключение к БД");
    firstProc->startDbProcess();
    if(!firstProc->testConnect()) return 18; // по кодам ошибок от 29.11.2012

    /// главное окно приложения
    MainWindow w;
    /// установка текста заголовка окна
    w.setWindowTitle(
                QString::fromUtf8("Тестовое приложения поиска по шаблону")
                     );
    w.show();
    
    int result = a.exec();
    delete firstProc;

    return result;
}
