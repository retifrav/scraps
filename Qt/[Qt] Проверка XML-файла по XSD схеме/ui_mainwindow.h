/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon 28. Jan 19:37:47 2013
**      by: Qt User Interface Compiler version 4.8.1
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
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit_schemaName;
    QToolButton *toolButton_getSchemaName;
    QLabel *label_2;
    QLineEdit *lineEdit_fileName;
    QToolButton *toolButton_getFileName;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_check;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(500, 110);
        MainWindow->setMinimumSize(QSize(500, 110));
        MainWindow->setMaximumSize(QSize(500, 110));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_schemaName = new QLineEdit(centralWidget);
        lineEdit_schemaName->setObjectName(QString::fromUtf8("lineEdit_schemaName"));

        gridLayout->addWidget(lineEdit_schemaName, 0, 1, 1, 1);

        toolButton_getSchemaName = new QToolButton(centralWidget);
        toolButton_getSchemaName->setObjectName(QString::fromUtf8("toolButton_getSchemaName"));

        gridLayout->addWidget(toolButton_getSchemaName, 0, 2, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_fileName = new QLineEdit(centralWidget);
        lineEdit_fileName->setObjectName(QString::fromUtf8("lineEdit_fileName"));

        gridLayout->addWidget(lineEdit_fileName, 1, 1, 1, 1);

        toolButton_getFileName = new QToolButton(centralWidget);
        toolButton_getFileName->setObjectName(QString::fromUtf8("toolButton_getFileName"));

        gridLayout->addWidget(toolButton_getFileName, 1, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_check = new QPushButton(centralWidget);
        pushButton_check->setObjectName(QString::fromUtf8("pushButton_check"));
        pushButton_check->setMinimumSize(QSize(100, 35));

        horizontalLayout->addWidget(pushButton_check);

        horizontalSpacer_2 = new QSpacerItem(38, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 3);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\272\320\260 XML-\321\204\320\260\320\271\320\273\320\260 \320\277\320\276 \321\201\321\205\320\265\320\274\320\265", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 \321\201\321\205\320\265\320\274\321\213:", 0, QApplication::UnicodeUTF8));
        toolButton_getSchemaName->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\321\217\320\265\320\274\321\213\320\271 XML-\321\204\320\260\320\271\320\273:", 0, QApplication::UnicodeUTF8));
        toolButton_getFileName->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        pushButton_check->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
