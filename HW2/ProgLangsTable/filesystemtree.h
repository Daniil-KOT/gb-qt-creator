#ifndef FILESYSTEMTREE_H
#define FILESYSTEMTREE_H

#include <QDialog>
#include <QFileSystemModel>
#include <QString>

namespace Ui {
class FilesystemTree;
}

class FilesystemTree : public QDialog
{
    Q_OBJECT

public:
    explicit FilesystemTree(QWidget *parent = nullptr);
    ~FilesystemTree();

private slots:
    void on_cancel_btn_clicked();

    void on_ok_btn_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

signals:
    void fileChosen(QString& path);

private:
    Ui::FilesystemTree *ui;
    QFileSystemModel* model_;
};

#endif // FILESYSTEMTREE_H
