#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

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
    void on_toolButton_clicked();

    void on_pushButton_readFile_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
