#ifndef COMPTABLE_H
#define COMPTABLE_H

#include <QMainWindow>
#include <QString>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class CompTable; }
QT_END_NAMESPACE

class CompTable : public QMainWindow
{
    Q_OBJECT

public:
    CompTable(QWidget *parent = nullptr);
    ~CompTable();

private slots:
    void on_add_btn_clicked();

    void on_pushButton_clicked();

private:
    QString getMac() const;
    void setInfo(QString name, QString ip, QString mac);

private:
    Ui::CompTable *ui;
    QStandardItemModel* model_;
    quint32 compNum_;
};
#endif // COMPTABLE_H
