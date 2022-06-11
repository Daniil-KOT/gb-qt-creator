#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QMenu>
#include <QTranslator>
#include <QContextMenuEvent>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(QWidget *parent = nullptr, QTranslator* translator = nullptr);

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
};

#endif // CUSTOMTEXTEDIT_H
