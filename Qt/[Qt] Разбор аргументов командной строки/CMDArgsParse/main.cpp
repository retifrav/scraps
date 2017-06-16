#include "opts.h"

using namespace std;

void foo(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    qDebug();

// --- проверка работы функций разбора аргументов командной строки -------------

//    // для проверки следует запустить утилиту с опциями: - a -b param -c
//    ArgsSet argsset("ab:c");
//    argsset.makeCMDoperable(argc, argv);
//    qDebug() << "Command is:" << argsset.get_command() << endl
//                << "and options are:" << argsset.get_gottenOptions();

// -----------------------------------------------------------------------------

    foo(argc, argv); // парсим первый раз
    foo(argc, argv); // парсим второй раз

    return 0;
}

void foo(int argc, char *argv[])
{
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

    qDebug() << "------- TEH END -------" << endl;
}
