#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>
#include <QtXmlPatterns>
#include <QMessageBox>
#include <QString>
#include <QFileDialog>

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
    void on_pushButton_check_clicked();
    void on_toolButton_getSchemaName_clicked();
    void on_toolButton_getFileName_clicked();

private:
    Ui::MainWindow *ui;

    QString schemaFileName;
    QString importFileName;
};

#endif // MAINWINDOW_H
