/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue 2. Apr 15:51:08 2013
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_startDir;
    QLineEdit *lineEdit_startDir;
    QToolButton *toolButton_startDir;
    QPushButton *pushButton_buildTree;
    QTreeView *treeView_dirTree;
    QPushButton *pushButton_exportTree2XML;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(502, 296);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_startDir = new QLabel(centralWidget);
        label_startDir->setObjectName(QString::fromUtf8("label_startDir"));

        horizontalLayout->addWidget(label_startDir);

        lineEdit_startDir = new QLineEdit(centralWidget);
        lineEdit_startDir->setObjectName(QString::fromUtf8("lineEdit_startDir"));
        lineEdit_startDir->setMinimumSize(QSize(250, 0));

        horizontalLayout->addWidget(lineEdit_startDir);

        toolButton_startDir = new QToolButton(centralWidget);
        toolButton_startDir->setObjectName(QString::fromUtf8("toolButton_startDir"));

        horizontalLayout->addWidget(toolButton_startDir);


        verticalLayout->addLayout(horizontalLayout);

        pushButton_buildTree = new QPushButton(centralWidget);
        pushButton_buildTree->setObjectName(QString::fromUtf8("pushButton_buildTree"));

        verticalLayout->addWidget(pushButton_buildTree);

        treeView_dirTree = new QTreeView(centralWidget);
        treeView_dirTree->setObjectName(QString::fromUtf8("treeView_dirTree"));

        verticalLayout->addWidget(treeView_dirTree);

        pushButton_exportTree2XML = new QPushButton(centralWidget);
        pushButton_exportTree2XML->setObjectName(QString::fromUtf8("pushButton_exportTree2XML"));

        verticalLayout->addWidget(pushButton_exportTree2XML);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\237\320\276\321\201\321\202\321\200\320\276\320\265\320\275\320\270\320\265 \321\201\321\202\321\200\321\203\320\272\321\202\321\203\321\200\321\213 \320\272\320\260\321\202\320\260\320\273\320\276\320\263\320\276\320\262 \320\262 XML", 0, QApplication::UnicodeUTF8));
        label_startDir->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\321\202\321\214 \321\201 \320\272\320\260\321\202\320\260\320\273\320\276\320\263\320\260:", 0, QApplication::UnicodeUTF8));
        lineEdit_startDir->setText(QApplication::translate("MainWindow", "C:\\", 0, QApplication::UnicodeUTF8));
        toolButton_startDir->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        pushButton_buildTree->setText(QApplication::translate("MainWindow", "1: \320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321\214 \320\264\320\265\321\200\320\265\320\262\320\276 \320\272\320\260\321\202\320\260\320\273\320\276\320\263\320\276\320\262", 0, QApplication::UnicodeUTF8));
        pushButton_exportTree2XML->setText(QApplication::translate("MainWindow", "2: \320\255\320\272\321\201\320\277\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\277\320\276\321\201\321\202\321\200\320\276\320\265\320\275\320\275\320\276\320\265 \320\264\320\265\321\200\320\265\320\262\320\276 \320\262 XML-\321\204\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
