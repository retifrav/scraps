#ifndef MODEL4TABLE_H
#define MODEL4TABLE_H

#include <QStandardItemModel>
#include <QtGui>

class Model4table : public QProxyModel
{
    Q_OBJECT
public:
    explicit Model4table(QObject *parent = 0);

    // переопределяем метод базового класса
    QVariant data(const QModelIndex &index, int role) const;    
};

#endif // MODEL4TABLE_H
