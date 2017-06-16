// -----------------------------------------------------------------------------

/**
  @mainpage Разбор аргументов командной строки

  Представляет собой систему классов, предназначенную для разбора аргументов
  командной строки, переданных утилите в качестве параметров запуска. Полученные
  аргументы разбираются на опции и их параметры, после чего создаётся объект
  команды, в который будут переданы результаты этого разбора.

  - @ref cmdformat
  - @ref usage
  - @ref example

  В состав системы входят следующие классы:
  - @ref ArgsSet - класс, осуществляющий разбор аргументов командной строки;
  - @ref Cmd - собственно команда, которая является результатом работы разбора
  аргументов командной строки. Содержит полученные из командной строки опции
  и метод запуска, в котором осуществляется её функционал;
  - @ref CmdBuilder - билдер команды. Нужен для построения команды;
  - @ref Opt - опция. Имеет имя, под которым она содержится в командной строке,
  а также "мнемоническое" имя;
  - @ref OptBuilder - билдер опции. Нужен для построения опции;
  - @ref OptProperty - свойства опции. Содержит признак обязательности опции,
  список опций, несовместимых с данной, и список опций, обязательных для данной;
  - @ref CmdOpt - командный объект. Содержит имя команды;
  - @ref Parser - парсер, объединяющий класс. Содержит в себе все предыдущие
  классы, занимается разбором командной строки и построением команды.
*/

// -----------------------------------------------------------------------------

/**
  @page cmdformat Формат командной строки

  Формат командной строки (параметры запуска утилиты) имеет следующий вид:
  @code
  <[путь/]утилита.exe> [команда] [<->опция] [параметры] [<->опция] [параметры] ...
  @endcode

  Здесь:
  - <b>команда</b> - имя команды, которую следует выполнить;
  - <b>опция</b> - имя опции, состоящее из одного символа. На то, что этот
  аргумент командной строки является именно опцией, указывает дефис, который
  ставится перед ней;
  - <b>параметры опции</b> - если опция не является флаговой, то после неё должны
  следовать её параметры. Если параметров два и более, то они заключаются в кавычки
  и разделяются запятыми.

  Пример строки параметров запуска, соответствующей заданному формату:
  @code
  utility.exe dosome -m now -o "4,2,5,3,1" -w
  @endcode
  Здесь:
  - <b>dosome</b> - команда;
  - <b>m</b> - первая опция команды, имеет параметр "now";
  - <b>o</b> - вторая опция команды, имеет параметры: "4", "2", "5", "3", "1";
  - <b>w</b> - третья опция команды, не имеет параметров (является флаговой).
*/

// -----------------------------------------------------------------------------

/**
  @page usage Инструкция по использованию

  Для использования необходимо включить в проект заголовочный файл:
  @code
    #include "opts.h"
  @endcode

  Затем следует произвести предварительную настройку в проекте:
    - унаследовать собственные классы команды и её билдера;
    - задать набор опций, которые будет использовать команда.

  После этого система будет реагировать только на те команды и опции
  в командной строке, которые были заданы на этапе предварительной настройки.
  Все остальные будут восприниматься как неверные команды и параметры и будут
  игнорироваться функциями разбора.

  Функционал команды определяется в методе run(), который необходимо переписать
  для каждой унаследованной от базовой команды.

  <b>Последовательность действий:</b>
  <ol>
    <li> унаследовать собственный билдер команды от базового @ref CmdBuilder :
    @code
        class CmdBuilderSpec2 : public CmdBuilder {public: Cmd *buildCmd();};
    @endcode
    <br>
    <li> унаследовать собственную команду от базовой @ref Cmd :
    @code
        class Cmd1 : public Cmd {public: bool run();};
    @endcode
    <br>
    <li> создать объекты опций и их свойств
        <br><br>
        - создать билдеры опций, которые могут быть у команды, указав имена
        опций в том виде, в каком они будут передаваться в командной строке:
        @code
            OptBuilder opt11("h"), opt12("s");
        @endcode
        <br>
        - создать объекты опций, передав в их конструкторы созданные
        на предыдущем шаге билдеры:
        @code
            Opt0 option11("help-mode", &opt11); // флаговая опция
            OptN option12("source", &opt12);    // опция с параметрами
        @endcode
        <br>
        - создать к каждой опции объект её свойств и объединить их в список:
        @code
            OptProperty optprops11(&option11, true), optprops12(&option12, true);
            QList<OptProperty> opts1;
            opts1 << optprops11 << optprops12;
        @endcode
        <br>
    <li> создать объект собственного билдера команды и передать его в командный
    объект:
    @code
        CmdBuilderSpec2 cmdbld1;
        CmdOpt comanda1("copy", &cmdbld1);
    @endcode
    <br>
    <li> создать объект @ref Parser и добавить в него командный объект и список
    объектов свойств опций:
    @code
        Parser parser;
        parser.addCmdAndOpts(&comanda1, opts1);
    @endcode
    <br>
    <li> выполнить метод parse(), передав ему в качестве параметров переменные
    argc и argv, в результате чего произойдёт разбор аргументов
    командной строки и создастся объект команды, который наполнится разобранными
    опциями:
    @code
        parser.parse(argc, argv);
    @endcode
  </ol>

  После этого команду можно вызывать на выполнение:
  @code
    if(parser.getCommand() != NULL) parser.getCommand()->run();
  @endcode

  Если команд несколько, то шаги 1-4 необходимо повторить для каждой команды
  и после этого переходить к шагу 5, на котором необходимо добавить все
  созданные командные объекты.

  В разделе @ref example приведено конкретное применение системы классов.
*/

// -----------------------------------------------------------------------------

/**
  @page example Пример использования

  <h2>Настройка команд и опций</h2>

  <h3>Наследование и переопределение классов команд и билдеров</h3>

  Наследование от базового класса команды:
  @code
  class Cmd0 : public Cmd {public: bool run();};
  class Cmd1 : public Cmd {public: bool run();};
  @endcode

  Переопределение метода run():
  @code
  // первая команда
  bool Cmd0::run()
  {
      qDebug() << "Cmd0-type command is running" << endl
               << "- opts-flags of the command are:" << getFlags() << endl
               << "- opts-with-params of the command are:" << getParams() << endl;
      return true;
  }
  // вторая команда
  bool Cmd1::run()
  {
      qDebug() << "Cmd1-type command is running" << endl
               << "- opts-flags of the command are:" << getFlags() << endl
               << "- opts-with-params of the command are:" << getParams() << endl;
      return true;
  }
  @endcode

  Наследование от базового класса билдера и переопределение метода:
  @code
  // наследование
  class CmdBuilderSpec1 : public CmdBuilder {public: Cmd *buildCmd();};
  class CmdBuilderSpec2 : public CmdBuilder {public: Cmd *buildCmd();};
  // переопределение
  Cmd *CmdBuilderSpec1::buildCmd() {return new Cmd0;}
  Cmd *CmdBuilderSpec2::buildCmd() {return new Cmd1;}
  @endcode

  <h3>Задание команд и наборов их опций</h3>

  После настройки команд и опций, они добавляются в объект парсера, и вызываются
  методы парсера.

  @code
    // создание объектов билдеров опций
    OptBuilder opt11("h"), opt12("s"),
            opt21("a"), opt22("b");



    // создание объектов опций
    // - для первой команды
    Opt0 option11("help-mode", &opt11);
    OptN option12("source", &opt12);
    // - для второй команды
    OptN option21("first option", &opt21);
    OptN option22("second option", &opt22);

    // создание свойств опций
    // - для первой команды
    OptProperty optprops11(&option11, true), optprops12(&option12, true),
    // - для второй команды
            optprops21(&option21, true), optprops22(&option22, true);

    // список опций со свойствами
    QList<OptProperty> opts1, opts2;
    // - для первой команды
    opts1 << optprops11 << optprops12;
    // - для второй команды
    opts2 << optprops21 << optprops22;

    // создание билдеров команды
    // - первая команда
    CmdBuilderSpec1 cmdbld1;
    CmdOpt comanda1("copy", &cmdbld1);
    // - вторая команда
    CmdBuilderSpec2 cmdbld2;
    CmdOpt comanda2("move", &cmdbld2);

    // создание парсера
    Parser parser;
    parser.addCmdAndOpts(&comanda1, opts1);
    parser.addCmdAndOpts(&comanda2, opts2);
    // запуск разбора командной строки и построение команды
    if(parser.parse(argc, argv))
    {
        // проверить, создалась ли команда, и выполнить её
        if(parser.getCommand() != NULL) parser.getCommand()->run();
        else qDebug() << "Command is empty - nothing to run";
    }
  @endcode

  Таким образом, были созданы следующие команды и наборы их опций:

  - команда <b>move</b>. Опции:
  <br><br>
  <table cellpadding=3 border=1 rules=all>
    <tr align=center><td>Имя опции</td><td>Обязательная</td><td>Флаговая</td></tr>
    <tr align=center><td><b>a</b></td><td>да</td><td>нет</td></tr>
    <tr align=center><td><b>b</b></td><td>да</td><td>нет</td></tr>
  </table>

  - команда <b>copy</b>. Опции:
  <br><br>
  <table cellpadding=3 border=1 rules=all>
    <tr align=center><td>Имя опции</td><td>Обязательная</td><td>Флаговая</td></tr>
    <tr align=center><td><b>h</b></td><td>да</td><td>да</td></tr>
    <tr align=center><td><b>s</b></td><td>да</td><td>нет</td></tr>
  </table><br>

  <h2>Запуск утилиты</h2>

  Формат передачи утилите параметров запуска приведён в разделе @ref cmdformat

  <h3>Примеры неверных параметров запуска</h3>

  - Запуск утилиты без параметров:
  @code
  > argtesting.exe
  ---
  error: "there are no options at all"
  @endcode

  - Запуск утилиты с неизвестной командой:
  @code
  > argtesting.exe badcomm -a -h
  ---
  error: "wrong command was gotten"
  @endcode

  - Запуск утилиты на выполнение команды "copy" с неизвестной опцией:
  @code
  > argtesting.exe copy -t
  ---
  copy: invalid option -- t
  error: "Necessary options are missing"
  @endcode

  - Запуск утилиты на выполнение команды "copy" без одной обязательной опции:
  @code
  > argtesting.exe copy -s 111
  ---
  error: "Necessary options are missing"
  @endcode

  - Запуск утилиты на выполнение команды "copy" с отсутствующими параметрами у опции:
  @code
  > argtesting.exe copy -h -s
  ---
  copy: option requires an argument -- s
  error: "Necessary options are missing"
  @endcode

  <h3>Примеры верных параметров запуска</h3>

  - Запуск утилиты на выполнение команды "copy":
  @code
  > argtesting.exe copy -h -s "first,second,third"
  ---
  Cmd0-type command is running
  - opts-flags of the command are: ("help-mode")
  - opts-with-params of the command are: QMap(("source", "third")("source", "second")("source", "first"))
  @endcode

  - Запуск утилиты на выполнение команды "move":
  @code
  > argtesting.exe move -b "1,2,3" -a "9,8,7"
  ---
  Cmd1-type command is running
  - opts-flags of the command are: ()
  - opts-with-params of the command are: QMap(("first option", "7")("first option", "8")("first option", "9")("second option", "3")("second option", "2")("second option", "1"))
  @endcode
*/
