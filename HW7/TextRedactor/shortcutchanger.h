#ifndef SHORTCUTCHANGER_H
#define SHORTCUTCHANGER_H

#include <QWidget>
#include <QList>
#include <QMenu>
#include <QString>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QKeySequence>
#include <QMap>
#include <QTranslator>

namespace Ui {
class ShortcutChanger;
}

class ShortcutChanger : public QWidget
{
    Q_OBJECT

public:
    ShortcutChanger(QWidget *parent = nullptr, QList<QMenu*> menus = QList<QMenu*>(), QTranslator *translator = nullptr);
    ~ShortcutChanger();

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    void setModel(QList<QMenu*> menus);

private slots:
    void changeShortcut(const QModelIndex &index);
    void onSequenceReady();

    void on_saveBtn_clicked();
    void on_cancelBtn_clicked();

signals:
    void sequenceReady();
    void shortcutsChanged(QMap<QString, QKeySequence> &shortcuts);

private:
    Ui::ShortcutChanger *ui;
    QStandardItemModel* model_;
    bool onChange_;
    QKeySequence* sequence_;
    QModelIndex changingItem_;
    QMap<QString, QKeySequence> shortcuts_;
    QTranslator *translator_;
};

#endif // SHORTCUTCHANGER_H
