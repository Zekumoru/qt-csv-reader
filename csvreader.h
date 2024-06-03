#ifndef CSVREADER_H
#define CSVREADER_H

#include "qnumberfilterproxymodel.h"

#include <QWidget>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class CSVReader;
}
QT_END_NAMESPACE

class CSVReader : public QWidget
{
    Q_OBJECT

public:
    CSVReader(QWidget *parent = nullptr);
    ~CSVReader();

private slots:
    void onOpenFileButtonClicked();
    void onSearchEditTextChanged(const QString&);

private:
    Ui::CSVReader *ui;
    QStandardItemModel *csvModel;
    QNumberFilterProxyModel *proxyModel;

    void parseFile(const QString& fileName);
    void parseLineValues(const QStringList& values, const QString& delim);
};
#endif // CSVREADER_H
