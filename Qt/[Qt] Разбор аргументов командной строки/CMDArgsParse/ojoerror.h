#ifndef OJOERROR_H
#define OJOERROR_H

#include <QString>
#include <QStringList>
#include <QDate>
#include <QNetworkInterface>
#include <stdlib.h>

class OjoError
{
public:
    OjoError();
    OjoError(const QString &messg, //обязательный
             int lvl = 1,  // ошибка
             const QString &code = "unknw-0",
             const QString &mdl = "unknown",
             // берёт из системного окружения имя текущего пользователя
             const QString &usr = getenv("USERNAME"),
             const QDateTime &dttm = QDateTime::currentDateTime(),
             // берёт адрес первого сетевого интерфейса
             const QString &messgIP = QNetworkInterface::allAddresses().first().toString(),
             const QString &messgXML = 0,
             const QStringList &logs = (const QStringList)0,
             const QStringList &fls = (const QStringList)0
             );
    // добавить текст последней ошибки
    void addLastError(const QString &txt);

    // получение значений полей
    QString getSenderUser() const;
    QDateTime getDateTime() const;
    int getMsgLevel() const;
    QString getMsgCode() const;
    QString getSenderModule() const;
    QStringList getPrivateLogs() const; //
    // concat all error in one
    QString getMsgText() const; //
    QString getMsgXML() const;
    QString getMsgIP() const;
    QStringList getPrivateFiles() const;
    QStringList getLastErrors() const;
    OjoError & operator  = (const OjoError &o);

    void redefine(const QString &messg,            // обязательный
                  int lvl = 1,                     // ошибка
                  const QString &code = "unknw-0",
                  const QString &mdl="unknown",
                  // берёт из системного окружения имя текущего пользователя
                  const QString &usr = getenv("USERNAME"),
                  const QDateTime &dttm = QDateTime::currentDateTime(),
                  // берёт адрес первого сетевого интерфейса
                  const QString &messgIP = QNetworkInterface::allAddresses().first().toString(),
                  const QString &messgXML = 0,
                  const QStringList &logs = (const QStringList)0,
                  const QStringList &fls = (const QStringList)0);

    void setOk(const QString &messg="Not Error!");

    bool isError();

protected:
    bool isOk;    

    // общие с OjoMsg
    QString msgText;
    int msgLevel;
    QString msgCode;
    QString msgSenderModule;
    QString msgSenderUser;
    QString msgIP;
    QDateTime msgDateTime;
    QString msgXML;
    QStringList privateLogs;
    QStringList privateFiles;
    // собственные
    QStringList lastErrors;
};

#endif // OJOERROR_H
