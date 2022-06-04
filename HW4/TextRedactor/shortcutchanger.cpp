#include "shortcutchanger.h"
#include "ui_shortcutchanger.h"

/*
 * Probably all of these could have been done less complex,
 * will try to update.
 */
ShortcutChanger::ShortcutChanger(QWidget *parent, QList<QMenu*> menus, QTranslator *translator) :
    QWidget(parent),
    ui(new Ui::ShortcutChanger),
    model_(new QStandardItemModel),
    onChange_(false),
    sequence_(nullptr),
    changingItem_(QModelIndex()),
    shortcuts_(QMap<QString, QKeySequence>()),
    translator_(translator)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Change shortcuts"));
    this->setWindowIcon(QIcon(":/Resources/icons/chng_shortcuts.png"));

    QList<QString> labels;
    labels.append(tr("Action"));
    labels.append(tr("Shortcut"));
    model_->setHorizontalHeaderLabels(labels);

    setModel(menus);

    ui->tableView->setModel(model_);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 200);

    connect(ui->tableView, &QTableView::clicked, this, &ShortcutChanger::changeShortcut);
    connect(this, &ShortcutChanger::sequenceReady, this, &ShortcutChanger::onSequenceReady);
}

ShortcutChanger::~ShortcutChanger()
{
    delete ui;
}

void ShortcutChanger::keyPressEvent(QKeyEvent *event)
{
    if (onChange_ && Qt::Key_Return != event->key())
    {
        if (sequence_)
        {
            delete sequence_;
            sequence_ = nullptr;
        }

        sequence_ = new QKeySequence(event->keyCombination());
    }

    // I don't really like this condition, but I didn't get how to avoid it
    // Probably timer can work?
    if (Qt::Key_Return == event->key() && 0 < sequence_->count())
        emit sequenceReady();
}

void ShortcutChanger::setModel(QList<QMenu*> menus)
{
    QList<QAction*> actions;
    QList<QStandardItem*> items;

    for (quint16 i = 0; i < menus.size(); ++i)
    {
        actions = menus.at(i)->actions();

        for (quint16 i = 0; i < actions.size(); ++i)
        {
            items.append(new QStandardItem(actions.at(i)->text()));
            items.append(new QStandardItem(actions.at(i)->shortcut().toString()));

            model_->appendRow(items);
            shortcuts_.insert(actions.at(i)->text(), actions.at(i)->shortcut());
            items.clear();
        }

        actions.clear();
    }
}

void ShortcutChanger::changeShortcut(const QModelIndex &index)
{
    if (0 == index.column())
        return;

    changingItem_ = index;
    onChange_ = true;
}

void ShortcutChanger::onSequenceReady()
{
    model_->clearItemData(changingItem_);

    if (shortcuts_.contains(model_->item(changingItem_.row())->text()))
    {
        if (!shortcuts_.key(*sequence_).isEmpty())
        {
            shortcuts_[model_->item(changingItem_.row())->text()] = QKeySequence();
            QList<QStandardItem*> items = model_->findItems(sequence_->toString(), Qt::MatchExactly, 1);

            // anyway we're sure that there will be an item and there'll be only one
            // however it's better to check to avoid unexpected situations
            if (!items.isEmpty())
            {
                items.at(0)->setText("");
            }
        }
        shortcuts_[model_->item(changingItem_.row())->text()] = *sequence_;
    }

    model_->setData(changingItem_, sequence_->toString());
    onChange_ = false;
}

void ShortcutChanger::on_saveBtn_clicked()
{
    emit shortcutsChanged(shortcuts_);
    close();
}

void ShortcutChanger::on_cancelBtn_clicked()
{
    close();
}
