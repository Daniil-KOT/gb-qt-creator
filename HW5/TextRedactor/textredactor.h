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
    void createNewFile();
    void open();
    void openReadOnly();

    void save();
    void saveAs();

    void openReference();

    void switchLanguage();
    void changeShortcuts();
    void shortcutsChanged(QMap<QString, QKeySequence> &shortcuts);
    void quit();

    void switchTheme();

private:
    Ui::TextRedactor *ui;
    QString filename_;
    QString filter_;
    QTranslator translator_;
    ShortcutChanger* scChanger_;

    struct Themes
    {
        Themes(QMenu* menu = nullptr, QAction* dark = nullptr, QAction* light = nullptr,
               QAction* fibres = nullptr, QAction* ubuntu = nullptr, QAction* cyan = nullptr)
            : menu_(menu),
              actSwitchDark(dark),
              actSwitchLight(light),
              actSwitchFibers(fibres),
              actSwitchUbuntu(ubuntu),
              actSwitchCyan(cyan)
        {
            if (menu_)
            {
                menu_->addAction(actSwitchDark);
                menu_->addAction(actSwitchLight);
                menu_->addAction(actSwitchFibers);
                menu_->addAction(actSwitchUbuntu);
                menu_->addAction(actSwitchCyan);
            }
        }

        QMenu* menu_;
        QAction* actSwitchDark;
        QAction* actSwitchLight;
        QAction* actSwitchFibers;
        QAction* actSwitchUbuntu;
        QAction* actSwitchCyan;
    } themes_;
};
#endif // TEXTREDACTOR_H
