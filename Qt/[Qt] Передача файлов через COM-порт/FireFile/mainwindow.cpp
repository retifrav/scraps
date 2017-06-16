#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functional.h"
#include <QKeyEvent>
#include <QtDebug>
#include <QFileDialog>

#define TIMEOUT 1000

// это будет там дальше нужно, чтобы определять, открывали ли уже порт
bool attmpt = false,
     portAlreadyChoosen = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_2->hide();

    ui->progressBar->setRange(0, 1000); // потому что прогресс приёма не определить

    logstr = tr("] <b>Запуск программы</b>");
    string2log(logstr);
    ui->log->append(tr("можете нажать <font color=blue><b>F1</b></font>"));
    logstr = "-----------------------------------------";
    ui->log->append(logstr);
    logstr = tr("<i>выберите порт и настройте его</i>");
    ui->log->append(logstr);
    logstr = tr("");
    ui->log->append(logstr);
    ui->lineEdit->setText(tr("выберите файл для передачи..."));
    ui->listenBtn->setEnabled(false);

    ComListener = new DemonPorta();

    config = new QSettings(qApp->applicationDirPath()+"/config.ini",
                         QSettings::IniFormat);

    // а просто так, впишем в конфиг нулевые значения. Реально, просто так
    config->setValue("port/Baud_Rate", 0);
    config->setValue("port/DataBits", 0);
    config->setValue("port/Parity", 0);
    config->setValue("port/StopBits", 0);
    config->setValue("file/fileToTransf", "");
    config->setValue("file/pathForSave", "C:/");

    // эта группа в лопосатых купальниках пишет в конфиг изменения настроек
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(BaudRateChanged(QString)));
    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)),
            this, SLOT(ParityChanged(int)));
    connect(ui->comboBox_4, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(DataBitsChanged(QString)));
    connect(ui->comboBox_5, SIGNAL(currentIndexChanged(int)),
            this, SLOT(StopBitsChanged(int)));

    // эти ребята дают Демону пользоваться GUI'ем
    connect(ComListener, SIGNAL(wannaUseLog(QString)),
            this, SLOT(toLog(QString)));
    connect(ComListener, SIGNAL(setProgr(int)),
            this, SLOT(setProgr(int)));
    connect(ComListener, SIGNAL(progrFin()),
            this, SLOT(finProgr()));

    // а вот он вываливается из своей функции, чтобы крикнуть Демону, куда являться
    connect(this, SIGNAL(curPortHandle(HANDLE)),
            ComListener, SLOT(setPortHandle(HANDLE)));
}

MainWindow::~MainWindow()
{delete ui;}

void DemonPorta::setPortHandle(HANDLE curPort)
{portHandle = curPort;}

void MainWindow::toLog(QString strToLog)
{logstr = strToLog; string2log(logstr);}

void MainWindow::setProgr(int val)
{if(ui->progressBar->value()<997) ui->progressBar->setValue(val);}

void MainWindow::finProgr()
{ui->progressBar->setValue(ui->progressBar->maximum());}

// --- сохранение изменений настроек соединения в файл -------------------------
void MainWindow::BaudRateChanged(QString param)
{config->setValue("port/Baud_Rate", param);}
void MainWindow::DataBitsChanged(QString param)
{config->setValue("port/DataBits", param);}
void MainWindow::ParityChanged(int param)
{config->setValue("port/Parity", param-1);}
void MainWindow::StopBitsChanged(int param)
{config->setValue("port/StopBits", param-1);}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- когда нажали на кнопку выбора файла для передачи ------------------------
void MainWindow::on_selectFile_clicked()
{ui->lineEdit->setText(file.getOpenFileName());
config->setValue("file/fileToTransf", ui->lineEdit->text());}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- когда нажали на кнопку открытия порта -----------------------------------
void MainWindow::on_tryToConnect_clicked()
{   
    // заставить выбрать один из портов
    if(ui->com1rbt->isChecked()==false && ui->com2rbt->isChecked()==false)
    {
        logstr = tr("] <font color=red>вы не выбрали порт</font>");
        string2log(logstr);
        return;
    }

    // заставить настроить порт
    if(ui->comboBox_2->currentIndex()==0
            || ui->comboBox_3->currentIndex()==0
            || ui->comboBox_4->currentIndex()==0
            || ui->comboBox_5->currentIndex()==0)
    {
        logstr = tr("] <font color=red>необходимо сначала настроить порт</font>");
        string2log(logstr);
        return;
    }

    // если порт уже открыт
    if(portAlreadyChoosen)
    {
        logstr = tr("] <font color=red>порт уже открыт</font>");
        string2log(logstr);
        return;
    }

    // смотря какой порт выбран
    if(ui->com1rbt->isChecked()==true) portstr = "COM1"; else portstr = "COM2";
    // попытаться открыть его
    workPort = OpenPort(portstr);
    attmpt = true;                 // попытка установления соединения состоялась
    portAlreadyChoosen = true;     // порт выбрали
    disableSettingsChange();

    if(workPort == INVALID_HANDLE_VALUE)
    {
        logstr = tr("] <font color=red>возможно, порт отключён или занят</font>");
        string2log(logstr);
        portAlreadyChoosen = false;
        return;
    }
    else
    {
        ui->label->hide();
        ui->label_2->show();
        ui->groupBox->setEnabled(true);
        ui->listenBtn->setEnabled(true);
        emit curPortHandle(workPort);
    }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- когда нажали на кнопку закрытия порта -----------------------------------
void MainWindow::on_tryToDisconnect_clicked()
{
    if(attmpt==false)
    {
        logstr = tr("] <u>чтобы продать что-нибудь ненужное, надо сначала купить что-нибудь ненужное</u>");
        string2log(logstr);
        return;
    }
    else
    {
     enableSettingsChange();
     attmpt = false;
     portAlreadyChoosen = false;

     if(CloseHandle(workPort))
     {
        logstr = "[";
        currtime = currtime.currentTime();
        logstr.append(currtime.toString());
        logstr.append(tr("] порт <b>") + portstr + tr("</b> закрыт"));
        ui->log->append(logstr);
        logstr = "---------------------------------------------------";
        if(ui->listenBtn->isChecked())
        {
            ui->listenBtn->setIcon(QIcon(":/picts/unlisten.gif"));
            ui->listenBtn->setChecked(false);
        }
        ui->listenBtn->setEnabled(false);
        ui->log->append(logstr);
        ui->label->show();
        ui->label_2->hide();
        ui->groupBox->setEnabled(false);
        ui->progressBar->setValue(0);
     }
     else
     {
        logstr = tr("] <font color=red>не удалось закрыть порт</font>");
        string2log(logstr);
     }
    }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- когда нажали F1 ---------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_F1: {
                      ab = new About(this);
                      ab->setWindowTitle(QString::fromUtf8("О программе"));
                      ab->setModal(true);
                      ab->show();
                      ab->raise();
                      ab->activateWindow();
                     }
        break;
     default: QWidget::keyPressEvent(event);
    }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- функция открытия порта --------------------------------------------------
HANDLE MainWindow::OpenPort(QString PortName)
{
    HANDLE PortToOpen = CreateFileA
            (
                PortName.toAscii(),           // имя порта
                GENERIC_READ | GENERIC_WRITE, // можно читать и писать
                0,                            // ни с кем портом не делиться
                NULL,                         // потомкам хэндл не доступен
                OPEN_EXISTING,                // открыть существующий порт
                FILE_ATTRIBUTE_NORMAL,        // без атрибутов
                0
            );

    if(PortToOpen==INVALID_HANDLE_VALUE)
    {
        logstr = tr("] <font color=red>не удалось открыть порт</font>");
        string2log(logstr);
    } else
    {
        logstr = "[";
        currtime = currtime.currentTime();
        logstr.append(currtime.toString());
        logstr.append(tr("] порт <b>") + portstr + tr("</b> открыт"));
        ui->log->append(logstr);

        // очистка буферов порта
        if(!PurgeComm(PortToOpen, PURGE_RXABORT | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR))
        {
            logstr = tr("] <font color=red>не удалось очистить буферы порта</font>");
            string2log(logstr);
            CloseHandle(PortToOpen);
            return INVALID_HANDLE_VALUE;
        } else {
            logstr = tr("] буферы порта очищены");
            string2log(logstr);
               }

        // настройка параметров порта
        DCB dcb;
        BOOL PortState;
        dcb.DCBlength = sizeof(DCB);
        PortState = GetCommState(PortToOpen,&dcb);
        dcb.BaudRate=config->value("port/Baud_Rate", "2400").value<int>();
        dcb.ByteSize=config->value("port/DataBits", "8").value<int>();
        dcb.Parity=config->value("port/Parity", "0").value<int>();
        dcb.StopBits=config->value("port/StopBits", "1").value<int>();

        if(!SetCommState(PortToOpen,&dcb))
        {
            logstr = tr("] <font color=red>не удалось настроить порт</font>");
            string2log(logstr);
            CloseHandle(PortToOpen);
            return INVALID_HANDLE_VALUE;
        } else {
            logstr = tr("] порт настроен");
            string2log(logstr);
               }

        // установка таймаутов
        COMMTIMEOUTS CommTimeOuts;
        CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
        CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
        CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
        CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
        CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;
        if(!SetCommTimeouts(PortToOpen, &CommTimeOuts))
        {
            logstr = tr("] <font color=red>ошибка выставления таймаутов</font>");
            string2log(logstr);
            CloseHandle(PortToOpen);
            return INVALID_HANDLE_VALUE;
        }
        else {
            logstr = tr("] таймауты выставлены");
            string2log(logstr);
             }
    }
    return PortToOpen;
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- функция рукопожатия -----------------------------------------------------
bool MainWindow::ComHandShake(HANDLE UsePort)
{
    DWORD write_counter;
    CHAR check = S_MARK;
    WriteFile
            (
                UsePort,        // имя порта
                &check,         // откуда писать в порт
                1,              // пишем один байт
                &write_counter, // куда считать число записанных байтов
                NULL            // это не нужно, короче
            );
    check = H_MARK;
    WriteFile(UsePort, &check, 1, &write_counter, NULL);
    DWORD read_counter;
    ReadFile
           (
                UsePort,       // имя порта
                &check,        // куда читать из порта
                1,             // сколько читать байтов
                &read_counter, // куда считать число считанных байтов
                NULL           // не надо оно нам, в общем
           );
    if(int(check)==S_MARK)
    {
        ReadFile(UsePort, &check, 1, &read_counter, NULL);
        if(int(check)==H_RES_MARK) return true;
    }
    return false;
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- функция отправки файла --------------------------------------------------
bool MainWindow::SendFile(QString FileName)
{
    // отправка маркера начала передачи
    CHAR C_Write = S_MARK;
    DWORD write_counter;
    WriteFile(workPort, &C_Write, 1, &write_counter, NULL);
    C_Write = NAME_MARK;
    WriteFile(workPort, &C_Write, 1, &write_counter, NULL);

    //передача имени файла
    QFileInfo FInfo(FileName);
    QString FName = FInfo.fileName();
    ui->progressBar->setMaximum(FInfo.size());
    int progrProgress = 0;
    logstr = tr("] <font color=blue>отправка файла</font> <b>") + FName + tr("</b>");
    string2log(logstr);
    logstr = tr("] передача имени файла...");
    string2log(logstr);
    byte A;
    short errors = 0;
    CHAR C_Read;
    for(int i = 0; i < FName.size(); i++)
    {
        do {
            C_Read = NO_CHAR;
            A = FName[i].toAscii();
            SendByte(A, workPort);
            ReadFile(workPort, &C_Read, 1, &write_counter, NULL);
            if(C_Read == NO_CHAR) errors++; else errors = 0;
            if(errors == MAX_ERRORS)
            {
                logstr = tr("] <font color=red>слишком много ошибок</font>");
                string2log(logstr);
                return false;
            }
            if(C_Read == B_MARK)
            {
                logstr = tr("] <font color=red>прервано отправителем</font>");
                string2log(logstr);
                C_Write = B_MARK;
                WriteFile(workPort, &C_Write, 1, &write_counter, NULL);
                return false;
            }
           } while(C_Read != A_MARK);
    }
    logstr = tr("] имя файла передано");
    string2log(logstr);
    logstr = tr("] передача самого файла...");
    string2log(logstr);


    // передача файла
    C_Write = DATA_MARK;
    WriteFile(workPort, &C_Write, 1, &write_counter, NULL);
    QFile FileToSend(FileName);
    if (FileToSend.open(QIODevice::ReadOnly))
    {
        char A;
        short errors = 0;
        for(int i=0; i<(FileToSend.size()); i++)
        {
            do {
                C_Read = NO_CHAR;
                QByteArray AByte = "";
                AByte = FileToSend.read(1);
                A = AByte[0];
                SendByte(A, workPort);
                ReadFile(workPort, &C_Read, 1, &write_counter, NULL);
                if(C_Read == NO_CHAR) errors++; else errors = 0;
                if(errors == MAX_ERRORS)
                {
                    logstr = tr("] <font color=red>слишком много ошибок при передаче</font>");
                    string2log(logstr);
                    FileToSend.close();
                    return false;
                }
                if(C_Read == B_MARK)
                {
                    logstr = tr("] <font color=red>передача прервана отправителем</font>");
                    string2log(logstr);
                    C_Write = B_MARK;
                    WriteFile(workPort, &C_Write, 1, &write_counter, NULL);
                    FileToSend.close();
                    return false;
                }
               } while(C_Read != A_MARK);
               progrProgress++;
               ui->progressBar->setValue(progrProgress);
        }
        FileToSend.close();
    }
    C_Write = S_MARK;
    WriteFile(workPort, &C_Write, 1, &write_counter, NULL);
    logstr = tr("] <font color=green>файл передан</font>");
    string2log(logstr);
    return true;
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- функция вывода информации в лог -----------------------------------------
void MainWindow::string2log (QString logstr)
{
    QString timeStr = "[";
    currtime = currtime.currentTime();
    timeStr.append(currtime.toString());
    timeStr.append(logstr);
    ui->log->append(timeStr);
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- блокирование/разблокирование изменения настроек порта -------------------
void MainWindow::disableSettingsChange()
{
    ui->groupBox_4->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
}
void MainWindow::enableSettingsChange()
{
    ui->groupBox_4->setEnabled(true);
    ui->comboBox_5->setEnabled(true);
    ui->comboBox_4->setEnabled(true);
    ui->comboBox_3->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- вызов демона порта! Пх’нглуи мглв’нафх Ктулху Р’льех вгах’нагл фхтагн ---
void DemonPorta::ListenPort(HANDLE ListenPort)
{
    CHAR C_Read;
    DWORD real_read;
    while(true)
    {
        C_Read = NO_CHAR;
        ReadFile(ListenPort, &C_Read, 1, &real_read, NULL);
        if(int(C_Read)==S_MARK)
        {
            ReadFile(ListenPort, &C_Read, 1, &real_read, NULL);
            // рукосжатие
            if(int(C_Read)==H_MARK)
            {
                C_Read = S_MARK;
                DWORD write_counter;

                WriteFile(ListenPort, &C_Read, 1, &write_counter, NULL);

                C_Read = H_RES_MARK;

                WriteFile(ListenPort, &C_Read, 1, &write_counter, NULL);
            }

            QString FileName;

            // читаем имя передаваемого файла
            if(int(C_Read)==NAME_MARK)
            {
                wannaUseLog(tr("] <font color=blue>рукопожатие...</font>"));
                wannaUseLog(tr("] <font color=green>рукопожатие прошло успешно</font>"));
                CHAR FullByte;
                short errors = 0;
                DWORD write_counter;
                do {
                    C_Read = NO_CHAR;
                    ReadFile(ListenPort, &C_Read, 1, &real_read, NULL);
                    if(C_Read == NO_CHAR) errors++; else errors = 0;
                    if(errors == MAX_ERRORS)
                    {
                        wannaUseLog(tr("] <font color=red>при передаче имени файла произошло слишком много ошибок</font>"));
                        break;
                    }
                    if(C_Read == B_MARK)
                    {
                        wannaUseLog(tr("] <font color=red>приём имени файла отменён</font>"));
                        break;
                    }
                    if (C_Read != DATA_MARK)
                    {
                        // получить имя
                        if(GetFullByte(C_Read, ListenPort, FullByte))
                        {
                            FileName.append(FullByte);
                            C_Read = A_MARK;
                            WriteFile(ListenPort, &C_Read,
                                      1, &write_counter, NULL);
                        } else {
                            C_Read = E_MARK;
                            WriteFile(ListenPort, &C_Read,
                                      1, &write_counter, NULL);
                               }
                    }
                } while (C_Read != DATA_MARK);
                wannaUseLog(tr("] <font color=blue>приём файла</font> <b>")
                            + FileName + tr("</b>"));
            }

            // чтение данных
            if(int(C_Read) == DATA_MARK)
            {
                DWORD write_counter;
                QString FullName = configDemona->
                        value("file/pathForSave", "C:/").toString();
                wannaUseLog(tr("] файл сохраняется в <b>")
                            + FullName + tr("</b>..."));
                FullName.append(tr("/") + FileName);
                int progrProgr = 0;
                QFile FileToSave(FullName);
                if(FileToSave.open(QIODevice::ReadWrite))
                {
                    CHAR FullByte;
                    short errors = 0;
                    do {
                        C_Read = NO_CHAR;
                        ReadFile(ListenPort, &C_Read, 1, &real_read, NULL);
                        if(C_Read == NO_CHAR) errors++; else errors = 0;
                        if(errors == MAX_ERRORS)
                        {
                            wannaUseLog(tr("] <font color=red>слишком много ошибок при передаче</font>"));
                            wannaUseLog(tr("] <font color=red>передача файла <b>")
                                        + FileName + tr("</b> была прервана</font>"));
                            break;
                        }
                        if(C_Read == B_MARK)
                        {
                            wannaUseLog(tr("] <font color=red>передача файла <b>")
                                        + FileName + tr("</b> была отменена</font>"));
                            break;
                        }
                        if (C_Read != S_MARK)
                        {
                            if(GetFullByte(C_Read, ListenPort, FullByte))
                            {
                                FileToSave.write(&FullByte, 1);
                                C_Read = A_MARK;
                                WriteFile(ListenPort, &C_Read, 1,
                                          &write_counter, NULL);
                            } else {
                                C_Read = E_MARK;
                                WriteFile(ListenPort, &C_Read, 1,
                                          &write_counter, NULL);
                                   }
                        }
                        progrProgr++;
                        emit setProgr(progrProgr);
                       } while (C_Read != S_MARK);
                    FileToSave.close();
                    if((C_Read != B_MARK) && (errors != MAX_ERRORS))
                    wannaUseLog(tr("] <font color=green>файл сохранён</font>"));
                    wannaUseLog(tr("] <font color=green><b>приём успешно завершён</b></font>"));
                    emit progrFin();
                }
            }
        }
    }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- начертили пентаграмму и читаем заклинание призыва Демона ----------------
void DemonPorta::run()
{
    configDemona = new QSettings(qApp->applicationDirPath()+"/config.ini",
                                               QSettings::IniFormat);
    ListenPort(portHandle); // демон призван!
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- когда нажали на кнопку Передать -----------------------------------------
void MainWindow::on_sendFile_clicked()
{
    ui->progressBar->setValue(0);

    if(ui->lineEdit->text()!=tr("выберите файл для передачи...")
            && !ui->lineEdit->text().isEmpty())
    {
        logstr = tr("] <font color=blue>рукопожатие...</font>");
        string2log(logstr);
        // рукопожимаем
        if(ComHandShake(workPort))
        {
            logstr = tr("] <font color=green>рукопожатие прошло успешно</font>");
            string2log(logstr);
            if(SendFile(config->value("file/fileToTransf",
                          qApp->applicationDirPath()+"/config.ini").toString()))
            {
                logstr = tr("] <font color=green><b>передача успешно завершилась<b></font>");
                string2log(logstr);
            }
            else
            {
                logstr = tr("] <font color=red><b>не удалось передать файл</font>");
                string2log(logstr);
            }
        }
        else
        {
            logstr = tr("] <font color=red>рукопожатие не удалось</font>");
            string2log(logstr);

            enableSettingsChange();
            attmpt = false;
            portAlreadyChoosen = false;

            if(CloseHandle(workPort))
            {
               logstr = "[";
               currtime = currtime.currentTime();
               logstr.append(currtime.toString());
               logstr.append(tr("] порт <b>") + portstr + tr("</b> закрыт"));
               ui->log->append(logstr);
               logstr = "---------------------------------------------------";
               if(ui->listenBtn->isChecked())
               {
                   ui->listenBtn->setIcon(QIcon(":/picts/unlisten.gif"));
                   ui->listenBtn->setChecked(false);
               }
               ui->listenBtn->setEnabled(false);
               ui->log->append(logstr);
               ui->label->show();
               ui->label_2->hide();
               ui->groupBox->setEnabled(false);
               ui->progressBar->setValue(0);
            }
            else
            {
               logstr = tr("] <font color=red>не удалось закрыть порт</font>");
               string2log(logstr);
            }
        }
    }
    else
    {
        logstr = tr("] <font color=red>вы не выбрали файл</font>");
        string2log(logstr);
    }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- когда нажали на кнопку прослушивания порта (призыва Демона!) ------------
void MainWindow::on_listenBtn_clicked()
{
    if(!ui->listenBtn->isChecked())
    {
        ui->progressBar->setValue(0);
        ComListener->terminate();
        ui->listenBtn->setIcon(QIcon(":/picts/unlisten.gif"));
    }
    else
    {
        ui->progressBar->setValue(0);
        ComListener->start();
        ui->listenBtn->setIcon(QIcon(":/picts/listen.gif"));
        config->setValue("file/pathForSave",
                         QFileDialog::getExistingDirectory(this,
                         tr("Выберите путь для сохранения"), "С:/",
                         QFileDialog::ShowDirsOnly
                         | QFileDialog::DontResolveSymlinks));
    }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// до семисот строк на хватало двух, вот теперь хватает
