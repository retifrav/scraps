/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun 29. Sep 19:51:41 2013
**      by: Qt User Interface Compiler version 4.6.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QPushButton *pushButton_send;
    QLineEdit *lineEdit_message;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_connect;
    QPushButton *pushButton_disconnect;
    QLabel *label_status;
    QTextEdit *textEdit_log;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(462, 275);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton_send = new QPushButton(centralWidget);
        pushButton_send->setObjectName(QString::fromUtf8("pushButton_send"));

        gridLayout->addWidget(pushButton_send, 1, 0, 1, 1);

        lineEdit_message = new QLineEdit(centralWidget);
        lineEdit_message->setObjectName(QString::fromUtf8("lineEdit_message"));

        gridLayout->addWidget(lineEdit_message, 1, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_connect = new QPushButton(centralWidget);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        pushButton_connect->setFont(font);
        pushButton_connect->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));

        horizontalLayout_2->addWidget(pushButton_connect);

        pushButton_disconnect = new QPushButton(centralWidget);
        pushButton_disconnect->setObjectName(QString::fromUtf8("pushButton_disconnect"));
        pushButton_disconnect->setEnabled(true);
        pushButton_disconnect->setFont(font);
        pushButton_disconnect->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_2->addWidget(pushButton_disconnect);

        label_status = new QLabel(centralWidget);
        label_status->setObjectName(QString::fromUtf8("label_status"));
        label_status->setMaximumSize(QSize(9999, 16777215));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_status->setFont(font1);
        label_status->setFrameShape(QFrame::Box);
        label_status->setFrameShadow(QFrame::Sunken);
        label_status->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_status);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 2);

        textEdit_log = new QTextEdit(centralWidget);
        textEdit_log->setObjectName(QString::fromUtf8("textEdit_log"));

        gridLayout->addWidget(textEdit_log, 2, 0, 1, 2);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\232\320\273\320\270 \320\225\320\275\321\202", 0, QApplication::UnicodeUTF8));
        pushButton_send->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214 \320\262\320\276\321\202 \321\215\321\202\320\276:", 0, QApplication::UnicodeUTF8));
        pushButton_connect->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", 0, QApplication::UnicodeUTF8));
        pushButton_disconnect->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", 0, QApplication::UnicodeUTF8));
        label_status->setText(QApplication::translate("MainWindow", "\321\201\321\202\320\260\321\202\321\203\321\201", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
