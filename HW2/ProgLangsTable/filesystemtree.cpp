#include "filesystemtree.h"
#include "ui_filesystemtree.h"

FilesystemTree::FilesystemTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilesystemTree),
    model_(new QFileSystemModel)
{
    ui->setupUi(this);
    model_->setRootPath("");
    ui->treeView->setModel(model_);
}

FilesystemTree::~FilesystemTree()
{
    delete ui;
}

void FilesystemTree::on_cancel_btn_clicked()
{
    close();
}


void FilesystemTree::on_ok_btn_clicked()
{
    QString path = model_->filePath(ui->treeView->currentIndex());
    emit fileChosen(path);
    close();
}

void FilesystemTree::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString path = model_->filePath(index);
    emit fileChosen(path);
    close();
}

