#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    filename = "test.xml";
    _model = new QStandardItemModel(0, 0, this);

    readXML();

    ui->tableView->setModel(_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readXML()
{
    QDomDocument document;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        document.setContent(&file);
        file.close();
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Ошибка открытия файла"),
                    tr("Не удалось открыть указанный файл.\n%1").
                    arg(file.errorString())
                    );
        return;
    }

    QStringList headerLabels;

    QList<QStandardItem *> idsList;
    QList<QStandardItem *> typesList;
    QList<QStandardItem *> statusesList;
    QList<QStandardItem *> dtssList;
    QList<QStandardItem *> idfsList;
    QList<QStandardItem *> msgssList;
    QList<QStandardItem *> mscodesList;
    QList<QStandardItem *> sendersList;
    QList<QStandardItem *> usersList;
    QList<QStandardItem *> ipsList;
    QList<QStandardItem *> origsList;
    QList<QStandardItem *> extsList;

    QDomElement xmlroot = document.firstChildElement();
    QDomNodeList messages = xmlroot.elementsByTagName("message");
    for(int i = 0; i < messages.count(); i++)
    {
        QDomElement message = messages.at(i).toElement();
        // извлечение 1 поля
        QDomNodeList ids = message.elementsByTagName("id_record");
        for(int h = 0; h < ids.count(); h++)
        {
            QDomElement id = ids.at(h).toElement();
            QStandardItem *iditem = new QStandardItem(id.text());
            idsList << iditem;
        }
        // извлечение 2 поля
        QDomNodeList types = message.elementsByTagName("msg_type_record");
        for(int h = 0; h < ids.count(); h++)
        {
            QDomElement type = types.at(h).toElement();
            QStandardItem *typeitem = new QStandardItem(type.text());
            typesList << typeitem;
        }
        // извлечение 3 поля
        QDomNodeList statuses = message.elementsByTagName("msg_status_record");
        for(int h = 0; h < statuses.count(); h++)
        {
            QDomElement status = statuses.at(h).toElement();
            QStandardItem *statusitem = new QStandardItem(status.text());
            statusesList << statusitem;
        }
        // извлечение 4 поля
        QDomNodeList dts = message.elementsByTagName("msg_date_record");
        for(int h = 0; h < dts.count(); h++)
        {
            QDomElement dt = dts.at(h).toElement();
            QStandardItem *dtitem = new QStandardItem(dt.text());
            dtssList << dtitem;
        }
        // извлечение 5 поля
        QDomNodeList idfs = message.elementsByTagName("id_file_record");
        for(int h = 0; h < idfs.count(); h++)
        {
            QDomElement idf = idfs.at(h).toElement();
            QStandardItem *idfitem = new QStandardItem(idf.text());
            idfsList << idfitem;
        }
        // извлечение 6 поля
        QDomNodeList msgs = message.elementsByTagName("msg_record");
        for(int h = 0; h < ids.count(); h++)
        {
            QDomElement msg = msgs.at(h).toElement();
            QStandardItem *msgitem = new QStandardItem(msg.text());
            msgssList << msgitem;
        }
        // извлечение 7 поля
        QDomNodeList mscodes = message.elementsByTagName("msg_code_record");
        for(int h = 0; h < mscodes.count(); h++)
        {
            QDomElement mscode = mscodes.at(h).toElement();
            QStandardItem *mscodeitem = new QStandardItem(mscode.text());
            mscodesList << mscodeitem;
        }
        // извлечение 8 поля
        QDomNodeList senders = message.elementsByTagName("msg_sender_record");
        for(int h = 0; h < senders.count(); h++)
        {
            QDomElement sender = senders.at(h).toElement();
            QStandardItem *senderitem = new QStandardItem(sender.text());
            sendersList << senderitem;
        }
        // извлечение 9 поля
        QDomNodeList users = message.elementsByTagName("msg_user_record");
        for(int h = 0; h < users.count(); h++)
        {
            QDomElement user = users.at(h).toElement();
            QStandardItem *useritem = new QStandardItem(user.text());
            usersList << useritem;
        }
        // извлечение 10 поля
        QDomNodeList ips = message.elementsByTagName("user_ip_record");
        for(int h = 0; h < ips.count(); h++)
        {
            QDomElement ip = ips.at(h).toElement();
            QStandardItem *ipitem = new QStandardItem(ip.text());
            ipsList << ipitem;
        }
        // извлечение 11 поля
        QDomNodeList origs = message.elementsByTagName("msg_original_record");
        for(int h = 0; h < origs.count(); h++)
        {
            QDomElement orig = origs.at(h).toElement();
            QStandardItem *origitem = new QStandardItem(orig.text());
            origsList << origitem;
        }
        // извлечение 12 поля
        QDomNodeList exts = message.elementsByTagName("msg_extdb_record");
        for(int h = 0; h < exts.count(); h++)
        {
            QDomElement ext = exts.at(h).toElement();
            QStandardItem *extitem = new QStandardItem(ext.text());
            extsList << extitem;
        }
    }
    _model->appendColumn(idsList);
    _model->appendColumn(typesList);
    _model->appendColumn(statusesList);
    _model->appendColumn(dtssList);
    _model->appendColumn(idfsList);
    _model->appendColumn(msgssList);
    _model->appendColumn(mscodesList);
    _model->appendColumn(sendersList);
    _model->appendColumn(usersList);
    _model->appendColumn(ipsList);
    _model->appendColumn(origsList);
    _model->appendColumn(extsList);

    headerLabels << "id_record"
                 << "msg_type_record"
                 << "msg_status_record"
                 << "msg_date_record"
                 << "id_file_record"
                 << "msg_record"
                 << "msg_code_record"
                 << "msg_sender_record"
                 << "msg_user_record"
                 << "user_ip_record"
                 << "msg_original_record"
                 << "msg_extdb_record";

    _model->setHorizontalHeaderLabels(headerLabels);
}
