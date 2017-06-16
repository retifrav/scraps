#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    modelXMLtree = new QFileSystemModel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_startDir_clicked()
{
    ui->lineEdit_startDir->setText(
                QFileDialog::getExistingDirectory (
                this,
                tr("Укажите каталог, с которого начнётся построение дерева"),
                tr("C:/"),
                QFileDialog::ShowDirsOnly| QFileDialog::ReadOnly
                ));
}

void MainWindow::on_pushButton_buildTree_clicked()
{
    modelXMLtree->setRootPath(ui->lineEdit_startDir->text());
    QModelIndex idx = modelXMLtree->index(ui->lineEdit_startDir->text());
    ui->treeView_dirTree->setModel(modelXMLtree);
    ui->treeView_dirTree->setRootIndex(idx);
    ui->treeView_dirTree->setColumnHidden(1, true);
    ui->treeView_dirTree->setColumnHidden(3, true);
    ui->treeView_dirTree->setColumnWidth(0, 200);
    ui->treeView_dirTree->expandToDepth(5);
}

void MainWindow::on_pushButton_exportTree2XML_clicked()
{
//    QFile exportFile(QFileDialog::getSaveFileName
//                         (
//                             this,
//                             tr("Укажите, куда сохранить файл экспорта"),
//                             QString(),
//                             tr("XML (*.xml)"))
//                         );
//    if(!exportFile.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QModelIndex index0, index1, index2;
    index0 = ui->treeView_dirTree->rootIndex();
    index1 = index0.child(0, 0);
//    index2 = index1.child(0, 0);
//    qDebug() << ui->treeView_dirTree->model()->data(index2).toString();

    int i = 0, y = 0;
    while(index1.isValid())
    {
        index1 = index0.child(i, 0);
        if(!index1.isValid()) continue;
        ui->treeView_dirTree->expand(index1);
        qDebug() << ui->treeView_dirTree->model()->data(index1).toString();

        if(index1.child(i,0).isValid())
        {
            index2 = index1.child(i, 0);
            while(index2.isValid())
            {
                index2 = index1.child(y, 0);
                if(!index2.isValid()) continue;
                ui->treeView_dirTree->expand(index2);
                qDebug() << "-- " << ui->treeView_dirTree->model()->data(index2).toString();
                y++;
            }
        }

        i++;
    }
}

void MainWindow::on_treeView_dirTree_clicked(const QModelIndex &index)
{
//    qDebug() << index;
}
