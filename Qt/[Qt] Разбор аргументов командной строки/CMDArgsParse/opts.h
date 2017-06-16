#ifndef OPTS_H
#define OPTS_H

#include <QDebug>
#include <QString>
#include <QMultiMap>
#include <QVariant>
#include <iostream>
#include "argsset.h"
#include "ojoerror.h"

// -----------------------------------------------------------------------------

/**
  @class Cmd
  @brief Команда.
  @details Предназначена для выполнения заданного функционала в зависимости
  от переданных опций и параметров. Для использования необходимо унаследовать
  собственный класс и переопределить в нём метод run().
*/
class Cmd
{
public:
    /// @brief выполнение команды
    /// @return true, если команда выполнена<br>false, если команда не выполнена
    virtual bool run();

    /// @brief добавление команде опции-флага
    /// @param mnemoName - опция-флаг
    /// @return true, если опция добавлена<br>false, если опция не добавлена
    bool addOpt(const QString &mnemoName);

    /// @brief добавление команде опции с параметрами
    /// @param mnemoName - опция с параметрами
    /// @param value - параметр опции
    /// @return true, если опция добавлена<br>false, если опция не добавлена
    bool addOpt(const QString &mnemoName, const QString &value);

    /// @brief получение списка опций-флагов
    /// @return список опций-флагов
    QList<QString> getFlags();

    /// @brief получение опций и их параметров
    /// @return ассоциативный массив опций и их параметров
    QMultiMap<QString, QString> getParams();

protected:
    /// опции-флаги
    QList<QString> flags;
    /// опции с параметрами
    QMultiMap<QString, QString> params;
};

class Cmd0 : public Cmd {public: bool run();};
class Cmd1 : public Cmd {public: bool run();};

// -----------------------------------------------------------------------------

/**
  @class CmdBuilder
  @brief Базовый класс билдера команды.
  @details Создаёт указатель на объект соответствующей команды.
*/
class CmdBuilder
{
public:
    /// @brief создание новой команды
    /// @return указатель на команду
    virtual Cmd *buildCmd();
};

// производные классы билдера команды
class CmdBuilderSpec1 : public CmdBuilder {public: Cmd *buildCmd();};
class CmdBuilderSpec2 : public CmdBuilder {public: Cmd *buildCmd();};

// -----------------------------------------------------------------------------

/**
  @class OptBuilder
  @brief Билдера опции.
  @details Служит для создания опции. Содержит имя опции, под которым
  она передаётся в командной строке.
*/
class OptBuilder
{
public:
    /// @brief конструктор по заданному имени опции
    /// @param givenkey - имя опции, под которым она передаётся в командной строке
    OptBuilder(const QString &givenkey);

    /// @brief добавление команде опции-флага
    /// @param c - команда, которой добавляется опция-флаг
    /// @return true, если опция-флаг добавлена<br>false, если опция-флаг не добавлена
    bool addOpt(Cmd *c);

    /// @brief добавление команде опции с параметрами
    /// @param c - команда, которой добавляется опция
    /// @param value - параметр опции
    /// @return true, если опция добавлена<br>false, если опция не добавлена
    bool addOpt(Cmd *c, const QString &value);

    /// @brief перегрузка оператора сравнения
    /// @param rhs - второй объект билдера
    /// @return true, если билдеры равны<br>false, если билдеры не равны
    bool operator==(const OptBuilder &rhs) const;

    /// @brief получить имя опции
    /// @return имя опции
    QString get_key();

protected:
    /// имя опции
    QString key;
};

// -----------------------------------------------------------------------------

/**
  @class Opt
  @brief Опция.
  @details Класс опции. Содержит имя, под которым опция передаётся
  в командной строке, "мнемоническое" имя опции и метод парсинга командной
  строки для добавления команде себя и своих параметров.
*/
class Opt
{
public:
    /// @brief конструктор по имени опции и её билдеру
    /// @param name - "мнемоническое" имя опции
    /// @param bld - билдер опции. Содержит реальное имя опции в командной строке
    Opt(const QString &name, OptBuilder *bld);

    /// @brief заполняет команду разобранными аргументами командной строки
    /// @param argsset - разобранные аргументы командной строки
    /// @param cmd - команда, в которую будут передаваться опции
    /// @return true, если передача состоялась<br>false, если передача не была осуществлена
    virtual bool parse(ArgsSet *argsset, Cmd* cmd);

    /// @brief перегрузка оператора сравнения
    /// @param rhs - второй объект опции
    /// @return true, если опции равны<br>false, если опции не равны
    bool operator==(const Opt &rhs) const;

    /// @brief получить реальное имя опции из командной строки
    /// @return реальное имя опции из командной строки
    QString get_OptName() const;

    /// @brief получить "мнемоническое" имя опции
    /// @return "мнемоническое" имя опции
    QString get_mnemoOptName() const;

    /// @brief показать, является опция флаговой или опцией с параметрами
    /// @return true, если это опция-флаг<br>false, если это опция с параметрами
    bool get_isItFlag() const;

protected:
    /// реальное имя опции в командной строке
    QString optName;
    /// "мнемоническое" имя опции
    QString mnemoName;
    /// количество параметров у опции
    int numberOfOptParams;
    /// билдер опции
    OptBuilder *optBuild;
    /// является опция флаговой или имеет параметры
    bool isItFlag;
};

/**
  @class Opt0
  @brief Опция-флаг.
  @details Класс флаговой опции. Это опция, которая не имеет параметров.
*/
class Opt0 : public Opt
{
public:
    /// @brief конструктор с наследованием от базового класса
    Opt0(const QString &name, OptBuilder *bld) : Opt(name, bld) {}
    bool parse(ArgsSet *argsset, Cmd* cmd);
};

/**
  @class OptN
  @brief Опция с параметрами.
  @details Класс опции с параметрами. У такой опции обязательно должны быть
  параметры.
*/
class OptN : public Opt
{
public:
    /// @brief конструктор с наследованием от базового класса
    OptN(const QString &name, OptBuilder *bld) : Opt(name, bld) {isItFlag = false;}
    bool parse(ArgsSet *argsset, Cmd* cmd);
};

// -----------------------------------------------------------------------------

/**
  @class CmdOpt
  @brief Командный объект.
  @details Ищет свою команду в командной строке. Если находит, то вызывает
  билдер для её построения.
*/
class CmdOpt
{
public:
    /// @brief конструктор по имени команды и билдеру команды
    /// @param cOpt - имя команды
    /// @param bld - билдер команды
    CmdOpt(const QString &cOpt, CmdBuilder *bld);

    /// @brief проверка присутствия команды в командной строке
    /// @param argsset - разобранные аргументы командной строки
    /// @return указатель на созданную команду
    Cmd *parse(ArgsSet *argsset);

    /// @brief получить имя команды
    /// @return имя команды
    QString get_cmdOpt() const;

    /// @brief получить билдер команды
    /// @return указатель на билдер команды
    CmdBuilder *get_cmdBuild();

protected:
    /// имя команды
    QString cmdOpt;
    /// билдер команды
    CmdBuilder *cmdBuild;
};

// -----------------------------------------------------------------------------

/**
  @class OptProperty
  @brief Свойства опции.
  @details Содержит набор свойств опции.
*/
class OptProperty
{
public:
    /// @brief конструктор
    /// @param o - опция
    /// @param isnecessary - признак необходимости
    /// @param necessary - список опций, необходимых для данной
    /// @param incompatible - список опций, несовместимых с данной
    OptProperty(
            Opt *o,
            bool isnecessary = true,
            const QList<Opt *> necessary = QList<Opt *>(),
            const QList<Opt *> incompatible = QList<Opt *>()
                );

    /// @brief получить опцию
    /// @return опция
    Opt *getOpt() const;

    /// @brief является ли опция обязательно
    /// @return true, если опция является обязательной<br>false, если не является
    bool isNeeded() const;

    /// @brief совместима ли указанная опция с данной
    /// @param oo - проверяемая опция
    /// @return true, если опция совместима с данной<br>false, если несовместима
    bool isMatch(Opt *oo); // можно ли использовать опцию совместно с данной

    /// @brief получить список опций, необходимых для данной
    /// @return список опций, необходимых для данной
    QList<Opt *> get_necessaryOpt();

    /// @brief получить список опций, несовместимых с данной
    /// @return список опций, несовместимых с данной
    QList<Opt *> get_incompatibleOpts();

private:
    /// опция
    Opt *opt;
    /// признак обязательности
    bool isNeed;
    /// список опций, обязательных для данной
    QList<Opt *> necessaryOpts;
    /// список опций, несовместимых с данной
    QList<Opt *> incompatibleOpts;
};

// -----------------------------------------------------------------------------

/**
  @class Parser
  @brief Главный класс разбора командной строки.
  @details Объединяющий класс. Содержит в себе все предыдущие классы
  и занимается разбором командной строки и построением команды.
*/
class Parser
{
public:
    /// конструктор по умолчанию
    Parser();

    /// @brief добавление новой команды и её опций
    /// @param cmd - команда
    /// @param opts - список её опций
    void addCmdAndOpts(CmdOpt *cmd, QList<OptProperty> opts);

    /// @brief разбор командной строки
    /// @param argc - количество аргументов командной строки
    /// @param argv - массив указателей на аргументы
    /// @return true, если разбор аргументов командной строки завершился успешно
    /// <br>false, если разбор завершился неудачей
    bool parse(int argc, char *argv[]);

    /// @brief вернуть построенную команду
    /// @return указатель на построенную команду
    Cmd *getCommand();

    /// @brief вернуть последнюю ошибку
    /// @return последняя полученная ошибка
    OjoError get_lastError();

    /// @brief сгенерировать строку шаблона аргументов командной строки
    /// @param cmOp - командный объект, из которого берутся опции команды
    /// @return строка шаблона для заданной команды
    QString genANDsetCmdArgTempl(CmdOpt *cmOp);

protected:
    /// временный указатель на командный объект
    CmdOpt *co;
    /// ассоциативный массив команд и их опций со свойствами
    QMap<CmdOpt*, QList<OptProperty> > cmndsAndOpts;
    /// итератор по массиву команд и их опций
    QMap<CmdOpt*, QList<OptProperty> >::iterator i;

    /// созданная и наполненная опциями команда, готовая к выполнению
    Cmd *currentCmd;
    /// разобранные аргументы командной строки
    ArgsSet *argsset;

    /// строка шаблона опций, по которой будет происходит разбор командной строки
    QString cmdArgTempl;
    /// ошибка
    OjoError lastError;
};

#endif // OPTS_H
