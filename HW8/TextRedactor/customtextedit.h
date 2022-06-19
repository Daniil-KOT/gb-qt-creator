#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QMenu>
#include <QTranslator>
#include <QContextMenuEvent>
#include <QPair>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(QWidget *parent = nullptr, QTranslator* translator = nullptr);

    QPair<QTextCharFormat, Qt::Alignment> getCopiedFormat() const;

    void setCopiedFormat(QPair<QTextCharFormat, Qt::Alignment> format);

    void copyFormat();
    void pasteFormat();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

public slots:
    void deleteText();
    void textSelected();
    void updateTranslator();

private:
    QMenu* menu_ = nullptr;
    QTranslator* translator_;
    bool actionsEnabled_ = false;
    QPair<QTextCharFormat, Qt::Alignment> copiedFmt_;
};

#endif // CUSTOMTEXTEDIT_H
