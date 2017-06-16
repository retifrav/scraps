#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include "errorscodes.h"
#include "dbprocess.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_search_clicked();

private:
    Ui::MainWindow *ui;

    QSqlQueryModel *_model;
};

#endif // MAINWINDOW_H
