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
    proxyModel = new QNumberFilterProxyModel(this);
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

    parseFile(fileName);
}

void CSVReader::onSearchEditTextChanged(const QString &text)
{
    proxyModel->setFilterFixedString(text);
}

void CSVReader::parseFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    ui->openedFileEdit->setText(fileName);

    QTextStream in(&file);
    csvModel->clear();
    bool isFirstLine = true;
    QString delim = ui->commaButton->isChecked() ? "," : ";";
    while (!in.atEnd()) {
        auto line = in.readLine();
        auto values = line.split(delim);

        if (isFirstLine) {
            csvModel->setHorizontalHeaderLabels(values);
            isFirstLine = false;
            continue;
        }

        parseLineValues(values, delim);
    }

    file.close();
    ui->tableView->resizeColumnsToContents(); // resize headers
}

void CSVReader::parseLineValues(const QStringList& values, const QString& delim)
{
    QList<QStandardItem *> items;
    // currentField is used for when there are
    // the delimeters in fields that have double quotes
    QString currentField;
    bool inQuotes = false;

    for (const QString &token : values) {
        if (inQuotes) {
            // since the getline in the while above already takes the delimeter
            // we bring it back here plus the token
            currentField += delim + token;
            // if the token ends in quotes, then we finished this field
            if (!token.isEmpty() && token.back() == '"') {
                inQuotes = false;
                // remove the ending double quotes
                auto value = currentField.mid(0, currentField.length() - 1);
                items.append(new QStandardItem(value));
                currentField = "";
            }

            continue;
        }

        // append any tokens including empty ones to table
        // except if it starts with quotes
        if (token.isEmpty() || token.front() != '"') {
            items.append(new QStandardItem(token));
            continue;
        }

        // handle fields in quotes
        inQuotes = true;
        currentField = token.mid(1); // remove starting double quotes

        if (token.back() == '"') {
            inQuotes = false;
            // remove the ending double quotes
            auto value = currentField.mid(0, currentField.length() - 1);
            items.append(new QStandardItem(value));
            currentField = "";
        }
    }

    // if there are no more tokens but we were inside a double quoted field
    // then we have to append it
    if (!currentField.isEmpty()) {
        // remove the ending double quotes
        auto value = currentField.mid(0, currentField.length() - 1);
        items.append(new QStandardItem(value));
    }

    csvModel->appendRow(items);
}

