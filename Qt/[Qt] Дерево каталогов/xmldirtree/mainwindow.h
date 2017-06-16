#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QtXml>
#include <QtXmlPatterns>

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
    void on_toolButton_startDir_clicked();
    void on_pushButton_buildTree_clicked();
    void on_pushButton_exportTree2XML_clicked();

    void on_treeView_dirTree_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *modelXMLtree;
};

#endif // MAINWINDOW_H
