#include "comptable.h"
#include "ui_comptable.h"

#include <QList>
#include <QString>
#include <QMessageBox>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>

#include <comptabledelegate.h>

CompTable::CompTable(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CompTable)
    , model_(new QStandardItemModel)
    , compNum_(1)
{
    ui->setupUi(this);
    QList<QString> columnNames;

    ui->ip->setValidator(new QIntValidator());
    ui->ip->setInputMask("009.009.009.009");
    ui->mac->setInputMask("NN:NN:NN:NN:NN:NN");

    // setting column names
    columnNames.append("№");
    columnNames.append("Computer Name");
    columnNames.append("IP Address");
    columnNames.append("MAC Address");

    model_->setHorizontalHeaderLabels(columnNames);

    // setting this computer info
    QString thisCompName = QHostInfo::localHostName();
    QString ip = QHostInfo::fromName(thisCompName).addresses().at(0).toString();
    QString mac = getMac();
    setInfo(thisCompName, ip, mac);
    ui->tableView->setModel(model_);
}

CompTable::~CompTable()
{
    delete ui;
}

// for example, if we had client - server arch
// this method could have been defined on the client side
QString CompTable::getMac() const
{
    foreach(QNetworkInterface netInt, QNetworkInterface::allInterfaces())
    {
        if (!(netInt.flags() & QNetworkInterface::IsLoopBack)
                && !netInt.humanReadableName().toLower().contains("vmware")
                && !netInt.humanReadableName().toLower().contains("virtual"))
        {
            return netInt.hardwareAddress();
        }
    }
    return QString();
}

// %mac can be replaced with getMac() method in our case
// but imagining we still have client - server arch
// %mac would be received from the client
void CompTable::setInfo(QString name, QString ip, QString mac)
{
    QList<QStandardItem*> columnsInfo;

    columnsInfo.append(new QStandardItem(QString::number(compNum_++)));
    columnsInfo.append(new QStandardItem(name));
    columnsInfo.append(new QStandardItem(ip));
    columnsInfo.append(new QStandardItem(mac));

    model_->appendRow(columnsInfo);
}

void CompTable::on_add_btn_clicked()
{

    if (8 > ui->ip->text().size() || 17 > ui->mac->text().size())
    {
        QMessageBox::information(this, "Warning", "Wrong format of IP or MAC!");
        return;
    }

    QString name;
    QString ip = ui->ip->text();
    QString mac = ui->mac->text();

    if (ui->name->text().isEmpty())
    {
        // performing reverse lookup
        QHostInfo info = QHostInfo::fromName(ip);
        if (info.error() == QHostInfo::NoError)
        {
            name = info.localHostName();
        }
        else
        {
            QMessageBox::information(this, "Warning", "Host not found!");
            return;
        }
    }
    else
    {
        name = ui->name->text();
    }

    setInfo(name, ip, mac);
}

void CompTable::on_add_empty_btn_clicked()
{
    QList<QStandardItem*> columnsInfo;

    columnsInfo.append(new QStandardItem(""));
    columnsInfo.append(new QStandardItem(""));
    columnsInfo.append(new QStandardItem(""));
    columnsInfo.append(new QStandardItem(""));

    model_->appendRow(columnsInfo);
}

void CompTable::on_paint_btn_clicked()
{
    QItemSelectionModel* selection = ui->tableView->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    for (QModelIndex idx : indexes)
        ui->tableView->setItemDelegateForRow(idx.row(), new CompTableDelegate());
}
