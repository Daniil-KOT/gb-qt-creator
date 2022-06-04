#ifndef TEXTREDACTOR_H
#define TEXTREDACTOR_H

#include <QMainWindow>

#include <QString>
#include <QTranslator>

#include "shortcutchanger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextRedactor; }
QT_END_NAMESPACE

class TextRedactor : public QMainWindow
{
    Q_OBJECT

public:
    TextRedactor(QWidget *parent = nullptr);
    ~TextRedactor();

private:
    void setLanguage(QString lang);

private slots:
    void open();
    void openReadOnly();

    void save();
    void saveAs();

    void openReference();

    void switchLanguage();
    void changeShortcuts();
    void shortcutsChanged(QMap<QString, QKeySequence> &shortcuts);
    void quit();

private:
    Ui::TextRedactor *ui;
    QString filename_;
    QString filter_;
    QTranslator translator_;
    ShortcutChanger* scChanger_;
};
#endif // TEXTREDACTOR_H
