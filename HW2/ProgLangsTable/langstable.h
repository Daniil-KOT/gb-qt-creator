#ifndef LANGSTABLE_H
#define LANGSTABLE_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMap>

#include "filesystemtree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LangsTable; }
QT_END_NAMESPACE

class LangsTable : public QMainWindow
{
    Q_OBJECT

public:
    LangsTable(QWidget *parent = nullptr);
    ~LangsTable();

private slots:

    void on_checkBox_stateChanged(int arg1);

    void on_del_btn_clicked();

    void on_add_btn_clicked();

    void on_toolButton_clicked();

    void setIconFilePath(QString& path);

    void on_up_btn_clicked();

    void on_down_btn_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void changeIcon(QStandardItem *item);

private:
    Ui::LangsTable *ui;
    QStandardItemModel* model_;
    const QMap <QString, QIcon> icons_ =
    {
        {"c++",         QIcon(":/Resources/c++.png")},
        {"c#",          QIcon(":/Resources/c-sharp.png")},
        {"java",        QIcon(":/Resources/java.png")},
        {"javascript",  QIcon(":/Resources/js.png")},
        {"php",         QIcon(":/Resources/php.png")},
        {"python",      QIcon(":/Resources/python.png")},
        {"default",     QIcon(":/Resources/default.png")}
    };
    FilesystemTree* tree_;
    bool isNameChanged_;
};
#endif // LANGSTABLE_H
