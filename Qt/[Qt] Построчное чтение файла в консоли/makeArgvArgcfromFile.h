#ifndef MAKEARGVARGCFROMFILE_H
#define MAKEARGVARGCFROMFILE_H

#include <QFile>
#include <QDebug>
#include <QString>

void readScenarioFile(QFile *incFile)
{
    // открываем файл
    if(incFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // тут будет очередная строка из файла
        QString line;
        // читать до конца файла
        while(!incFile->atEnd())
        {
            line = incFile->readLine().trimmed(); // прочитали строку
            qDebug() << line;
        }

        incFile->close();
    } else qDebug() << "file doesn't exist";
}

#endif // MAKEARGVARGCFROMFILE_H
