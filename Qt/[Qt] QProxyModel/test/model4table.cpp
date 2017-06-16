#include "model4table.h"

Model4table::Model4table(QObject *parent) :
    QProxyModel(parent)
{
}

QVariant Model4table::data(const QModelIndex &index, int role) const
{
    QVariant value = QProxyModel::data(index, role);

    // в какую бы ячейку ни вписали цифру 1, отображаться будет слово "внимание"
    if(value.isValid() && value.toInt() == 1)
        return tr("внимание");

    // если в любую ячейку первого (реально - второго) столбца...
    if(role == Qt::BackgroundColorRole)
    {
        // впишут слово "ololo", вся строка с этой ячейкой покрасится в красный
        return (index.sibling(index.row(), 1).data(Qt::DisplayRole) == "ololo")
                ? QColor(Qt::red) : value;
    } else return value;
}
