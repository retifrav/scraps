#include "argsset.h"
#include <QDebug>

using namespace std;

ArgsSet::ArgsSet()
{
    optionsAmount = 0;
    templAmount = 0;
    token = ',';
}

ArgsSet::ArgsSet(QString tmppl)
{
    templ4getopt = tmppl;
    optionsAmount = 0;
    templAmount = 0;
    set_optionsTemp();
    token = ',';
}

void ArgsSet::changeToken(QChar newtoken) {token = newtoken;}

void ArgsSet::set_optionsTemp()
{
    // заполнить список параметров из строки шаблона
    for(int x=0; x<templ4getopt.size(); x++)
        if(templ4getopt[x] != ':')
        {
            optionsTemp << templ4getopt.mid(x, 1);
            templAmount++;
        }
}

void ArgsSet::set_options(QString val) {options << val;}
void ArgsSet::set_params(QString val) {params << val;}
void ArgsSet::inc_optionsAmount() {optionsAmount++;}

void ArgsSet::set_gottenOptions(QString key, QString val)
{gottenOptions.insert(key, val);}

QString ArgsSet::get_optionsTempAt(int cnt) {return optionsTemp.at(cnt);}
QString ArgsSet::get_optionsAt(int cnt) {return options.at(cnt);}
QString ArgsSet::get_paramsAt(int cnt) {return params.at(cnt);}
QString ArgsSet::get_command() {return command;}

void ArgsSet::makeCMDoperable(int argc, char *argv[])
{
    options.clear();
    params.clear();

    // отключить вывод в консоль сообщений о неизвестных опциях
    // opterr = 0;

    int param = 0, cnt;

    if(argc > 1)
    {
        command = argv[1];
        if(templ4getopt == QString()) return;
        int argcNEW = argc - 1;
        char *argvNEW[argcNEW];
        for(cnt = 1; cnt < argc; cnt++) {argvNEW[cnt-1] = argv[cnt];}

        while((param = getopt(argcNEW, argvNEW, templ4getopt.toAscii())) != -1)
        {
            for(cnt = 0; cnt < templAmount; cnt++)
            {
                if(param == get_optionsTempAt(cnt)[0].toAscii())
                {
                    set_options((QString)optopt);
                    set_params((QString)optarg);
                    inc_optionsAmount();
                }
            }
        }
    }
    else return;

    for(cnt = 0; cnt < optionsAmount; cnt++)
    set_gottenOptions(get_optionsAt(cnt), get_paramsAt(cnt));
//    qDebug() << gottenOptions << endl;
    findAndSet_optsFlagsOrWithParams();
//    qDebug() << optsFlags;
//    qDebug() << optsWithParams;
    
	// обнуление индекса обратно на начало командной строки 
	optind = 0;
}

void ArgsSet::findAndSet_optsFlagsOrWithParams()
{
    QMultiMap<QString, QString>::iterator i;
    for (i = gottenOptions.begin(); i != gottenOptions.end(); ++i)
    {
        if(i.value().isEmpty()) optsFlags.append(i.key());
        else optsWithParams.append(i.key());
    }
//    qDebug() << optsFlags;
//    qDebug() << optsWithParams;
}

bool ArgsSet::isOptFlag(QString optName)
{if(optsFlags.contains(optName)) return true; else return false;}

bool ArgsSet::isOptWithParam(QString optName)
{if(optsWithParams.contains(optName)) return true; else return false;}

QString ArgsSet::paramsForTheOpt(QString optName)
{return gottenOptions.value(optName);}

QChar ArgsSet::get_token() {return token;}
