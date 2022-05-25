#include "langstable.h"
#include "ui_langstable.h"

#include <QDebug>
#include <QMessageBox>

LangsTable::LangsTable(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LangsTable)
    , model_(new QStandardItemModel)
    , tree_(new FilesystemTree)
{
    ui->setupUi(this);

    model_->appendRow(new QStandardItem(icons_["C++"], "C++"));
    model_->appendRow(new QStandardItem(icons_["C#"], "C#"));
    model_->appendRow(new QStandardItem(icons_["Java"], "Java"));
    model_->appendRow(new QStandardItem(icons_["JavaScript"], "JavaScript"));
    model_->appendRow(new QStandardItem(icons_["PHP"], "PHP"));
    model_->appendRow(new QStandardItem(icons_["Python"], "Python"));

    connect(tree_, &FilesystemTree::fileChosen, this, &LangsTable::setIconFilePath);
    connect(model_, &QStandardItemModel::itemChanged, this, &LangsTable::changeIcon);
    ui->listView->setModel(model_);
}

LangsTable::~LangsTable()
{
    delete ui;
}

void LangsTable::on_checkBox_stateChanged(int arg1)
{
    if (Qt::CheckState::Checked == arg1)
        ui->listView->setViewMode(QListView::IconMode);
    else
        ui->listView->setViewMode(QListView::ListMode);
}

void LangsTable::on_del_btn_clicked()
{
    if (model_->checkIndex(ui->listView->currentIndex()))
    {
        model_->removeRow(ui->listView->currentIndex().row());
    }
}

void LangsTable::on_add_btn_clicked()
{
    QString name;
    if (!ui->langName->text().isEmpty())
        name = ui->langName->text();
    else
    {
        QMessageBox::information(this, "Warning", "Language name cannot be empty!");
        return;
    }

    if (ui->pathToIcon->text().isEmpty())
        model_->appendRow(new QStandardItem(icons_["Default"], name));
    else
        model_->appendRow(new QStandardItem(QIcon(ui->pathToIcon->text()), name));
}

void LangsTable::on_toolButton_clicked()
{
    tree_->setModal(true);
    tree_->exec();
}

void LangsTable::setIconFilePath(QString &path)
{
    ui->pathToIcon->clear();
    ui->pathToIcon->setText(path);
}

/*
 *
 * selection cursor acts weird when slots
 * on_up_btn_clicked and on_down_btn_clicked are invoked
 * can you please advice how to improve it?
 *
 */
void LangsTable::on_up_btn_clicked()
{
    if (ui->listView->currentIndex().row() <= 0)
        return;
    auto tmp = model_->takeRow(ui->listView->currentIndex().row());
    model_->insertRow(ui->listView->currentIndex().row() - 1, tmp);
}

void LangsTable::on_down_btn_clicked()
{
    if (ui->listView->currentIndex().row() == model_->rowCount() - 1)
        return;
    auto tmp = model_->takeRow(ui->listView->currentIndex().row());
    model_->insertRow(ui->listView->currentIndex().row() + 1, tmp);
}

/*
 *
 * Don't know how else I can stop infinite loop
 * of itemChanged -> changeIcon calls
 *
 */
void LangsTable::on_listView_doubleClicked(const QModelIndex &index)
{
    isNameChanged_ = true;
}

void LangsTable::changeIcon(QStandardItem *item)
{
    if (isNameChanged_)
    {
        isNameChanged_ = false;

        if (icons_.contains(item->text()))
            item->setIcon(icons_[item->text()]);
        else
            item->setIcon(icons_["Default"]);
    }
}

