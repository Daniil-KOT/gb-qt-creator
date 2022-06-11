#include "customtextedit.h"

CustomTextEdit::CustomTextEdit(QWidget *parent, QTranslator* translator)
    : QTextEdit(parent),
      translator_(translator)
{
    menu_ = new QMenu(this);

    QAction* undo = new QAction(tr("Undo"));
    QAction* redo = new QAction(tr("Redo"));
    QAction* cut = new QAction(tr("Cut"));
    QAction* copy = new QAction(tr("Copy"));
    QAction* paste = new QAction(tr("Paste"));
    QAction* del = new QAction(tr("Delete"));
    QAction* selectAll = new QAction(tr("Select all"));

    undo->setShortcut(QKeySequence::Undo);
    redo->setShortcut(QKeySequence::Redo);
    cut->setShortcut(QKeySequence::Cut);
    copy->setShortcut(QKeySequence::Copy);
    paste->setShortcut(QKeySequence::Paste);
    selectAll->setShortcut(QKeySequence::SelectAll);

    undo->setEnabled(false);
    redo->setEnabled(false);
    cut->setEnabled(false);
    copy->setEnabled(false);
    del->setEnabled(false);

    menu_->addAction(undo);
    menu_->addAction(redo);
    menu_->addSeparator();

    menu_->addAction(cut);
    menu_->addAction(copy);
    menu_->addAction(paste);
    menu_->addSeparator();

    menu_->addAction(del);
    menu_->addAction(selectAll);

    connect(undo, &QAction::triggered, this, &QTextEdit::undo);
    connect(this, &QTextEdit::undoAvailable, undo, &QAction::setEnabled);

    connect(redo, &QAction::triggered, this, &QTextEdit::redo);
    connect(this, &QTextEdit::redoAvailable, redo, &QAction::setEnabled);

    connect(cut, &QAction::triggered, this, &QTextEdit::cut);
    connect(copy, &QAction::triggered, this, &QTextEdit::copy);
    connect(paste, &QAction::triggered, this, &QTextEdit::paste);
    connect(del, &QAction::triggered, this, &CustomTextEdit::deleteText);
    connect(selectAll, &QAction::triggered, this, &QTextEdit::selectAll);

    connect(this, &QTextEdit::selectionChanged, this, &CustomTextEdit::textSelected);
}

void CustomTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    menu_->exec(event->globalPos());
}

void CustomTextEdit::deleteText()
{
    this->textCursor().removeSelectedText();
}

void CustomTextEdit::textSelected()
{
    if ((this->textCursor().hasSelection() || this->textCursor().hasComplexSelection())
            && !actionsEnabled_)
    {
        QList<QAction* > actions = menu_->actions();

        for (QAction* action : actions)
        {
            if (tr("Cut") == action->text() ||
                tr("Copy") == action->text() ||
                tr("Delete") == action->text())
            {
                action->setEnabled(true);
            }
        }

        actionsEnabled_ = true;
    }
    else if (!this->textCursor().hasSelection() &&
             !this->textCursor().hasComplexSelection() &&
             actionsEnabled_)
    {
        QList<QAction* > actions = menu_->actions();

        for (QAction* action : actions)
        {
            if (tr("Cut") == action->text() ||
                tr("Copy") == action->text() ||
                tr("Delete") == action->text())
            {
                action->setEnabled(false);
            }
        }

        actionsEnabled_ = false;
    }
}

void CustomTextEdit::updateTranslator()
{
    QList<QAction* > actions = menu_->actions();

    for (QAction* action : actions)
    {
        action->setText(tr(action->text().toStdString().c_str()));
    }
}
