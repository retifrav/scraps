#ifndef ARGSSET_H
#define ARGSSET_H

#include <QString>
#include <QStringList>
#include <QMultiMap>
#include <iostream>
#include <unistd.h>

/**
  @class ArgsSet
  @brief Разбор аргументов командной строки.
  @details Представляет возможности по работе с аргументами командной строки:
  получение имени команды и разбор опций и их параметров. Для выделения
  из аргументов командной строки опций и их параметров используется функция
  getopt() из стандартной библиотеки "unistd.h". На вход функция принимает argc,
  argv и строку шаблона, по которой она будет анализировать аргументы командной
  строки.

  В процессе выполнения функции getopt() заполняется список опций (options)
  и (если есть) список их параметров (params). Далее из этих списков создаётся
  ассоциативный массив gottenOptions, который, однако, скрыт от доступа извне.

  Для доступа к разобранным опциям и их параметрам предусмотренны
  соответствующие методы.
*/

class ArgsSet
{
public:
    /// конктруктор по умолчанию, не имеет строки шаблона
    ArgsSet();
    /// @brief конструктор со строкой шаблона
    /// @param  tmppl - строка шаблона, которая будет передаваться в getopt()
    ArgsSet(QString tmppl);

    /// @brief разбор полученных аргументов командной строки по шаблону
    /// @param argc - количество аргументов командной строки
    /// @param argv - массив указателей на аргументы
    void makeCMDoperable(int argc, char *argv[]);
    /// сменить токен, которым разделяются параметры для опции внутри кавычек
    /// @param newtoken - символ, которым будут разделяться параметры опции в строке
    void changeToken(QChar newtoken);

    /// @brief получить имя команды
    /// @return имя команды
    QString get_command();

    /// @brief получить токен
    /// @return символ, которым разделяются параметры опции в строке
    QChar get_token();

    /// @brief показывает, присутствует ли запрашиваемая опция в списке флагов
    /// @param optName - запрашиваемая опция
    /// @return true, если присутствует<br>false, если отсутствует
    bool isOptFlag(QString optName);

    /// @brief показывает, присутствует ли запрашиваемая опция в списке опций с параметрами
    /// @param optName - запрашиваемая опция
    /// @return true, если присутствует<br>false, если отсутствует
    bool isOptWithParam(QString optName);

    /// @brief для запрашиваемой опции возвращает строку её параметров
    /// @param optName - запрашиваемая опция
    /// @return строка параметров для запрошенной опции
    QString paramsForTheOpt(QString optName);

protected:
    QString get_optionsTempAt(int cnt);
    QString get_optionsAt(int cnt);
    QString get_paramsAt(int cnt);

    /// распределяет полученные опции на опции-флаги и опции с параметрами
    void findAndSet_optsFlagsOrWithParams();

    /// заполнить список параметров из строки шаблона
    void set_optionsTemp();
    /// заполнение списка полученных опций
    void set_options(QString val);
    /// заполнение списка параметров полученных опций
    void set_params(QString val);
    /// изменение числа реально переданных параметров
    void inc_optionsAmount();
    /// заполнение ассоциативного массива опций и их параметров
    void set_gottenOptions(QString key, QString val);

private:
    /// имя команда (что лежит в argv[1])
    QString command;
    /// строка шаблона, которая подставляется третьим параметром в getopt
    QString templ4getopt;
    /// параметры, полученные из разбора строки шаблона
    QStringList optionsTemp;
    /// опции, переданные в командной строке
    QStringList options;
    /// строки параметров опций
    QStringList params;
    /// число реальных параметров
    int optionsAmount;
    /// число шаблонных параметров
    int templAmount;
    /// токен, который раздеяет параметры одной опции внутри кавычек
    QChar token;

    /// ассоциативный массив, опция-параметры
    QMultiMap<QString, QString> gottenOptions;

    /// выделенные из общей массы опции-флаги
    QStringList optsFlags;
    /// выделенные из общей массы опции с параметрами
    QStringList optsWithParams;


};

#endif // ARGSSET_H
