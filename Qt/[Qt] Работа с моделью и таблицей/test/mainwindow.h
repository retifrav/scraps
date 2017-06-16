#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

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
    // обработчик нажатия на кнопку Добавить строку
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    // объявляем модель как член класса, чтобы её видели все функции класса
    QStandardItemModel *model;
};

#endif // MAINWINDOW_H
