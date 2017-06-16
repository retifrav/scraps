#include "ojoerror.h"

OjoError::OjoError()
{
    isOk = false;

    msgDateTime = QDateTime::currentDateTime();
    msgLevel = 0;
}

OjoError::OjoError(const QString &messg, // обязательный
                   int lvl,              // ошибка
                   const QString &code,
                   const QString &mdl,
                   const QString &usr,
                   const QDateTime &dttm,
                   const QString &messgIP,
                   const QString &messgXML,
                   const QStringList &logs,
                   const QStringList &fls)
{
    isOk = false;

    msgSenderUser = usr;
    msgDateTime = dttm;
    msgLevel = lvl;
    msgCode = code;
    msgSenderModule = mdl;
    msgText = messg;
    msgIP = messgIP;
    msgXML = messgXML; // нужно заполнить по умолчанию
    privateLogs = logs;
    privateFiles = fls;
}

void OjoError::redefine(const QString &messg,
                        int lvl,
                        const QString &code,
                        const QString &mdl,
                        const QString &usr,
                        const QDateTime &dttm,
                        const QString &messgIP,
                        const QString &messgXML,
                        const QStringList &logs,
                        const QStringList &fls)
{
    isOk = false;    

    msgSenderUser = usr;
    msgDateTime = dttm;
    msgLevel = lvl;
    msgCode = code;
    msgSenderModule = mdl;
    msgText = messg;
    msgIP = messgIP;
    msgXML = messgXML; // нужно заполнить по умолчанию
    privateLogs = logs;
    privateFiles = fls;
}

void OjoError::setOk(const QString &messg)
{
    redefine(messg);
    isOk = true;
}

bool OjoError::isError()
{
    return (!isOk);
}

void OjoError::addLastError(const QString &txt)
{
    lastErrors << txt;
    msgText.append(txt);
}

QString OjoError::getMsgText() const {return msgText;}
int OjoError::getMsgLevel() const {return msgLevel;}
QString OjoError::getMsgCode() const {return msgCode;}
QString OjoError::getSenderModule() const {return msgSenderModule;}
QString OjoError::getSenderUser() const {return msgSenderUser;}
QDateTime OjoError::getDateTime() const {return msgDateTime;}
QString OjoError::getMsgXML() const {return msgXML;}
QString OjoError::getMsgIP() const {return msgIP;}
QStringList OjoError::getPrivateLogs() const {return privateLogs;}
QStringList OjoError::getPrivateFiles() const {return privateFiles;}
QStringList OjoError::getLastErrors() const {return lastErrors;}

OjoError & OjoError::operator  = (const OjoError &o)
{
    /*if(o.isError())
            this->redefine(o.msgText,
                    o.msgLevel,
                    o.msgCode,
                    o.msgSenderModule,
                    o.msgSenderUser,
                    o.msgDateTime,
                    o.msgXML,
                    o.privateLogs,
                    o.privateFiles);
        else
            this->setOk(); */
    this->isOk = o.isOk;

    this->lastErrors = o.lastErrors;
    this->msgCode = o.msgCode;
    this->msgDateTime = o.msgDateTime;
    this->msgLevel = o.msgLevel;
    this->msgSenderModule = o.msgSenderModule;
    this->msgSenderUser = o.msgSenderUser;
    this->msgText = o.msgText;
    this->msgXML = o.msgXML;
    this->privateFiles = o.privateFiles;
    this->privateLogs = o.privateLogs;

    return *this;
}

