#ifndef PUSTAKAWANMODEL_H
#define PUSTAKAWANMODEL_H

#include <QSqlTableModel>

class PustakawanModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit PustakawanModel(QObject *parent = 0);

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
};

#endif // PUSTAKAWANMODEL_H
