#include "opts.h"

// -----------------------------------------------------------------------------

bool Cmd::addOpt(const QString &mnemoName)
{
    if(flags.contains(mnemoName)) return false;
    else flags.append(mnemoName);
    return true;
}

bool Cmd::addOpt(const QString &mnemoName, const QString &value)
{
    params.insert(mnemoName, value);
    return true;
}

bool Cmd::run() {qDebug() << "command is running"; return true;}

QList<QString> Cmd::getFlags() {return flags;}
QMultiMap<QString, QString> Cmd::getParams() {return params;}

bool Cmd0::run()
{
    qDebug() << "Cmd0-type command is running" << endl
             << "- opts-flags of the command are:" << getFlags() << endl
             << "- opts-with-params of the command are:" << getParams() << endl;
    return true;
}

bool Cmd1::run()
{
    qDebug() << "Cmd1-type command is running" << endl
             << "- opts-flags of the command are:" << getFlags() << endl
             << "- opts-with-params of the command are:" << getParams() << endl;
    return true;
}

// -----------------------------------------------------------------------------

Cmd *CmdBuilder::buildCmd() {return new Cmd;}
Cmd *CmdBuilderSpec1::buildCmd() {return new Cmd0;}
Cmd *CmdBuilderSpec2::buildCmd() {return new Cmd1;}

// -----------------------------------------------------------------------------

Opt::Opt(const QString &name, OptBuilder *bld) : optBuild(bld)
//Opt::Opt(const QString &name)
{
    optName = bld->get_key();
    mnemoName = name;
    numberOfOptParams = 0;
    isItFlag = true;
}

bool Opt::operator==(const Opt &rhs) const
{
    return
            optName == rhs.optName &&
            mnemoName == rhs.mnemoName &&
            optBuild == rhs.optBuild &&
            numberOfOptParams == rhs.numberOfOptParams;
}

QString Opt::get_OptName() const {return optName;}
QString Opt::get_mnemoOptName() const {return mnemoName;}
bool Opt::get_isItFlag() const {return isItFlag;}
bool Opt::parse(ArgsSet *argsset, Cmd *cmd) {return false;}

bool Opt0::parse(ArgsSet *argsset, Cmd *cmd)
{
    // проверка присутствия опции в КС
    if(!argsset->isOptFlag(optName)) return false;

    // добавление опции-флага объекту-команде
    cmd->addOpt(mnemoName);

//    qDebug() << "Option-flag is:" << optName;

    return true;
}

bool OptN::parse(ArgsSet *argsset, Cmd *cmd)
{
    // проверка присутствия опции в КС
    if(!argsset->isOptWithParam(optName)) return false;

    // извлечение из КС параметров опции и подсчёт их количества
    // параметры в строке разделены токеном
    // (?) унести разбор токена в ArgsSet

    QString optParamsString = argsset->paramsForTheOpt(optName);
    QStringList optParams;
    int x, y = 0, z = 0;
    QChar tok = argsset->get_token();
    for(x = 0; x < optParamsString.length(); x++)
    {
        z++;
        if(optParamsString[x] == tok)
        {
            optParams << optParamsString.mid(y, z-1);
            y = x + 1;
            z = 0;
            numberOfOptParams++;
        }
        if(x == optParamsString.length()-1)
        {
            optParams << optParamsString.mid(y, z);
            numberOfOptParams++;
        }
    }

    // добавление опции и её параметров объекту-команде
    for(x = 0; x < numberOfOptParams; x++)
        {cmd->addOpt(mnemoName, optParams.at(x));}

//    qDebug() << "For the option" << optName << "there are" << numberOfOptParams
//             << "parameters, and them are: " << optParams;

    return true;
}

// -----------------------------------------------------------------------------

OptBuilder::OptBuilder(const QString &givenkey)
{key = givenkey;}

QString OptBuilder::get_key() {return key;}

bool OptBuilder::addOpt(Cmd *c)
{if(c == NULL) return false; return c->addOpt(key);}

bool OptBuilder::addOpt(Cmd *c, const QString &value)
{if(c == NULL) return false; return c->addOpt(key, value);}

bool OptBuilder::operator==(const OptBuilder &rhs) const
{return key == rhs.key;}

// -----------------------------------------------------------------------------

CmdOpt::CmdOpt(const QString &cOpt, CmdBuilder *bld) : cmdBuild(bld)
{cmdOpt = cOpt;}

Cmd *CmdOpt::parse(ArgsSet *argsset)
{
    // если такой опции нет, то вернуть пустой указатель
    if(cmdOpt != argsset->get_command()) return NULL;
    else return cmdBuild->buildCmd();
}

QString CmdOpt::get_cmdOpt() const {return cmdOpt;}

// -----------------------------------------------------------------------------

OptProperty::OptProperty(Opt *o,
                         bool isnecessary,
                         const QList<Opt *> necessary,
                         const QList<Opt *> incompatible): opt(o)
{
    isNeed = isnecessary;
    necessaryOpts = necessary;
    incompatibleOpts = incompatible;
}

bool OptProperty::isMatch(Opt *oo)
{
    // если опция есть в списке несовместимых, то вернуть false
    if(incompatibleOpts.contains(oo)) return false;
    else return true;
}

Opt *OptProperty::getOpt() const {return opt;}
bool OptProperty::isNeeded() const {return isNeed;}
QList<Opt *> OptProperty::get_necessaryOpt() {return necessaryOpts;}
QList<Opt *> OptProperty::get_incompatibleOpts() {return incompatibleOpts;}

// -----------------------------------------------------------------------------

Parser::Parser()
{
    currentCmd = NULL;
    argsset = NULL;
    cmdArgTempl = QString();
    co = NULL;
}

void Parser::addCmdAndOpts(CmdOpt *cmd, QList<OptProperty> opts)
{cmndsAndOpts.insert(cmd, opts);}

QString Parser::genANDsetCmdArgTempl(CmdOpt *cmOp)
{
    for (int x = 0; x < cmndsAndOpts.value(cmOp).size(); x++)
    {
        cmdArgTempl.append(cmndsAndOpts.value(cmOp)[x].getOpt()->get_OptName());
        if(!cmndsAndOpts.value(cmOp)[x].getOpt()->get_isItFlag()) cmdArgTempl.append(":");
    }

    // можно посмотреть сгенерированную строку
//    qDebug() << "what we got:" << cmdArgTempl;

    return cmdArgTempl;
}


Cmd *Parser::getCommand() {return currentCmd;}
OjoError Parser::get_lastError() {return lastError;}

bool Parser::parse(int argc, char *argv[])
{
    // (+) добавить проверку отсутствия несовместимых опций
    // (+) добавить проверку присутствия всех обязательных опций для каждой опции

    // если в командной строке ничего не передали, то сразу выходим
    if(argc == 1)
    {
        lastError.redefine("there are no options at all");
        qDebug() << "error:" << lastError.getMsgText();
        return false;
    }

    // одноразовый разбор командной строки для получения имени команды
    argsset = new ArgsSet;
    argsset->makeCMDoperable(argc, argv);

    // поиск среди имеющихся команд той, которая передана в argv[1]
    for(i = cmndsAndOpts.begin(); i != cmndsAndOpts.end(); ++i)
        if(i.key()->get_cmdOpt() == argsset->get_command())
            co = i.key();

    delete argsset;
    argsset = NULL;

    // можно посмотреть правильность отлова команд
//    if(co == NULL) qDebug() << "wrong command was gotten";
//    else qDebug() << co->get_cmdOpt();

    if(co == NULL)
    {
        lastError.redefine("wrong command was gotten");
        qDebug() << "error:" << lastError.getMsgText();
        delete co;
        return false;
    }

    argsset = new ArgsSet(genANDsetCmdArgTempl(co));
    argsset->makeCMDoperable(argc, argv);

    // можно посмотреть, что разобралось из аргументов команной строки
//    qDebug() << argsset->get_gottenOptions();

    currentCmd = co->parse(argsset);
    // (-) лишняя проверка
    if(currentCmd == NULL)
    {
        lastError.redefine("There was no any known command in the cmdline");
        qDebug() << "error:" << lastError.getMsgText();
        return false;
    }

    // наполнение команды и одновременная проверка на наличие всех обязательных опций
    for (int x = 0; x < cmndsAndOpts.value(co).size(); x++)
        if(!cmndsAndOpts.value(co)[x].getOpt()->parse(argsset, currentCmd)
                && cmndsAndOpts.value(co)[x].isNeeded())
        {
            lastError.redefine("Necessary options are missing");
            qDebug() << "error:" << lastError.getMsgText();
            delete currentCmd;
            currentCmd = NULL;
            delete argsset;
            argsset = NULL;
            co = NULL;
            return false;
        }

    delete argsset;
    argsset = NULL;
    co = NULL;
    return true;
}
