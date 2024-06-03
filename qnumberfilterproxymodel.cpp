#include "qnumberfilterproxymodel.h"

#include <QStandardItemModel>

QNumberFilterProxyModel::QNumberFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

bool QNumberFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    auto leftData = sourceModel()->data(left);
    auto rightData = sourceModel()->data(right);

    bool isLeftNumeric = false;
    bool isRightNumeric = false;

    auto leftNumber = leftData.toDouble(&isLeftNumeric);
    auto rightNumber = rightData.toDouble(&isRightNumeric);
    if (isLeftNumeric && isRightNumeric) {
        return leftNumber < rightNumber;
    }

    auto leftString = leftData.toString();
    auto rightString = rightData.toString();

    return QString::compare(leftString, rightString) < 0;
}
