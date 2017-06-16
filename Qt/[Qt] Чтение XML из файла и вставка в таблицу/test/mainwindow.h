#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QtXml>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void readXML();
    
private:
    Ui::MainWindow *ui;

    QStandardItemModel *_model;
    QString filename;
};

#endif // MAINWINDOW_H
