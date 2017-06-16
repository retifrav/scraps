#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
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

public slots:
    // действие, которое будет выполняться по сигналу от таймера
    void updateTime();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
