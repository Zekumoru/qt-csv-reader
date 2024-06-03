#ifndef QNUMBERFILTERPROXYMODEL_H
#define QNUMBERFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class QNumberFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT;

public:
    explicit QNumberFilterProxyModel(QObject *parent = nullptr);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // QNUMBERFILTERPROXYMODEL_H
