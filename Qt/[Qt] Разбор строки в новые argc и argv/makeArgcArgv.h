#ifndef MAKEARGVARGCFROMFILE_H
#define MAKEARGVARGCFROMFILE_H

#include <QDebug>
#include <QStringList>

/*
  Смотрит строку и выцепляет из неё по пробелам опции и их параметры
  множественные параметры (которые в кавычках) выцепляет одной строкой
  так было нужно. А на отдельные параметры разбирает потом другая фигня
  в другом месте. Формат строки строго такой, как в main. То есть, кавычки
  идут вплотную к опции, без пробела, и внутри кавычек пробелов тоже нет
*/

char **makeArgsFromString(QString lineFromFile, int *newArgc, char **newArgv)
{
    int x, y = 0, z = 0, kav;
    QStringList templist;
    templist << "-exename-";
    for(x = 0; x < lineFromFile.length(); x++)
    {
        z++;
        if(lineFromFile[x] == ' ')
        {
            templist << lineFromFile.mid(y, z-1);
            y = x + 1;
            z = 0;
        }
        if(lineFromFile[x+1] == '\"')
        {
            x = kav = x + 2;
            while(lineFromFile[x] != '\"') {x++; z++;}
            templist << lineFromFile.mid(kav, z);
            z = 0;
            x++;
            y = x + 1;
        }
        if(x == lineFromFile.length()-1) {templist << lineFromFile.mid(y, z);}
    }

    // можно вывести и посмотреть, что он насобирал
    // for(x = 0; x < templist.size(); x++) {qDebug() << templist.at(x);}

    // формирование инициализация newArgc и newArgv
    *newArgc = templist.size();
    newArgv = new char*[templist.size() + 1];
    for (int i = 0; i < templist.size(); i++)
    {
        newArgv[i] = new char[strlen(templist.at(i).toStdString().c_str())+1];
        memcpy(newArgv[i], templist.at(i).toStdString().c_str(),
               strlen(templist.at(i).toStdString().c_str())+1);
    }
    newArgv[templist.size()] = ((char)NULL);

    return newArgv;
}

#endif // MAKEARGVARGCFROMFILE_H
