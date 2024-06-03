#include "csvreader.h"
#include "./ui_csvreader.h"

#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>

CSVReader::CSVReader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CSVReader)
{
    ui->setupUi(this);
    connect(ui->openFileButton, &QPushButton::clicked, this, &CSVReader::onOpenFileButtonClicked);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &CSVReader::onSearchEditTextChanged);

    csvModel = new QStandardItemModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(csvModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1); // Filter all columns

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true); // Enable column sorting
}

CSVReader::~CSVReader()
{
    delete ui;
}

void CSVReader::onOpenFileButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open CSV"), "", tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    ui->openedFileEdit->setText(fileName);

    QTextStream in(&file);
    csvModel->clear();
    bool isFirstLine = true;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(",");

        if (isFirstLine) {
            csvModel->setHorizontalHeaderLabels(values);
            isFirstLine = false;
            continue;
        }

        QList<QStandardItem *> items;
        for (const QString &text : values) {
            items.append(new QStandardItem(text));
        }
        csvModel->appendRow(items);
    }

    file.close();

    ui->tableView->resizeColumnsToContents(); // resize headers
}

void CSVReader::onSearchEditTextChanged(const QString &text)
{
    proxyModel->setFilterFixedString(text);
}
