#include "pustakawanmodel.h"

PustakawanModel::PustakawanModel(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable("tbl_pustakawan");
}

QVariant PustakawanModel::data(const QModelIndex &idx, int role) const
{
    if(role == Qt::CheckStateRole) {
        if(idx.column() == 1) {
            const QModelIndex &tmp = index(idx.row(), 5);
            return QSqlTableModel::data(tmp, Qt::EditRole).toInt() ? Qt::Checked : Qt::Unchecked;
        }
    }

    if(role == Qt::DisplayRole) {
        if(idx.column() == 5) {
            return QString();
        }
    }

    return QSqlTableModel::data(idx, role);
}
