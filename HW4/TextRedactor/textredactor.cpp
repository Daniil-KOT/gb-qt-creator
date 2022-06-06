#include "textredactor.h"
#include "ui_textredactor.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QAction>
#include <QKeySequence>
#include <QDir>

TextRedactor::TextRedactor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextRedactor)
    , filename_("")
    , filter_("Text documents(*.txt)")
{
    ui->setupUi(this);

    this->setWindowTitle("Simple Text Redactor v0.1");
    this->setWindowIcon(QIcon(":/Resources/icons/mainwindow.png"));

    QAction* actCreateNew = new QAction(QIcon(":/Resources/icons/new_file.png"), tr("&New file"), this);
    QAction* actOpen = new QAction(QIcon(":/Resources/icons/open.png"), tr("&Open"), this);
    QAction* actOpenReadOnly = new QAction(QIcon(":Resources/icons/open.png"), tr("&Open read only"), this);
    QAction* actSave = new QAction(QIcon(":/Resources/icons/save.png"), tr("&Save"), this);
    QAction* actSaveAs = new QAction(QIcon(":/Resources/icons/save_as.png"), tr("&Save as"), this);
    QAction* actReference = new QAction(QIcon(":/Resources/icons/info.png"), tr("&Reference"), this);
    QAction* actSwitchLang = new QAction(QIcon(":/Resources/icons/switch_lang.png"), tr("&Switch language"), this);
    QAction* actChangeShortcuts = new QAction(QIcon(":/Resources/icons/chng_shortcuts.png"), tr("&Change shortcuts"), this);
    QAction* actQuit = new QAction(QIcon(":/Resources/icons/quit.png"), tr("&Quit"), this);

    actCreateNew->setShortcut(QKeySequence::New);
    actOpen->setShortcut(QKeySequence::Open);
    actOpenReadOnly->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_O);
    actSave->setShortcut(QKeySequence::Save);

#ifdef Q_OS_WIN

    actSaveAs->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    actQuit->setShortcut(Qt::CTRL | Qt::Key_Q);

#elif Q_OS_UNIX

    actSaveAs->setShortcut(QKeySequence::SaveAs); // does not work with KDE Plasma
    actQuit->setShortcut(QKeySequence::Quit);

#endif

    actReference->setShortcut(QKeySequence::HelpContents);
    actSwitchLang->setShortcut(Qt::Key_F2);
    actChangeShortcuts->setShortcut(Qt::Key_F3);

    ui->menuFile->addAction(actCreateNew);
    ui->menuFile->addAction(actOpen);
    ui->menuFile->addAction(actOpenReadOnly);
    ui->menuFile->addAction(actSave);
    ui->menuFile->addAction(actSaveAs);

    ui->menuInformation->addAction(actReference);

    ui->menuOptions->addAction(actSwitchLang);
    ui->menuOptions->addAction(actChangeShortcuts);
    ui->menuOptions->addAction(actQuit);

    QList<QMenu*> menus;
    menus.append(ui->menuFile);
    menus.append(ui->menuInformation);
    menus.append(ui->menuOptions);

    scChanger_ = new ShortcutChanger(nullptr, menus, &translator_);

    setLanguage("en");

    connect(actCreateNew, &QAction::triggered, this, &TextRedactor::createNewFile);
    connect(actOpen, &QAction::triggered, this, &TextRedactor::open);
    connect(actOpenReadOnly, &QAction::triggered, this, &TextRedactor::openReadOnly);
    connect(actSave, &QAction::triggered, this, &TextRedactor::save);
    connect(actSaveAs, &QAction::triggered, this, &TextRedactor::saveAs);
    connect(actReference, &QAction::triggered, this, &TextRedactor::openReference);
    connect(actSwitchLang, &QAction::triggered, this, &TextRedactor::switchLanguage);
    connect(actChangeShortcuts, &QAction::triggered, this, &TextRedactor::changeShortcuts);
    connect(scChanger_, &ShortcutChanger::shortcutsChanged, this, &TextRedactor::shortcutsChanged);
    connect(actQuit, &QAction::triggered, this, &TextRedactor::quit);
}

TextRedactor::~TextRedactor()
{
    delete ui;
}

void TextRedactor::setLanguage(QString lang)
{
    if (translator_.load(":/Textredactor_" + lang))
    {
        if (scChanger_)
        {
            if (scChanger_->isEnabled())
                scChanger_->close();

            delete scChanger_;
            scChanger_ = nullptr;
        }
        qApp->installTranslator(&translator_);
    }

    QList<QAction*> actions = ui->menuFile->actions();

    ui->menuFile->setTitle(tr("File"));
    actions.at(0)->setText(tr("&New file"));
    actions.at(1)->setText(tr("&Open"));
    actions.at(2)->setText(tr("&Open read only"));
    actions.at(3)->setText(tr("&Save"));
    actions.at(4)->setText(tr("&Save as"));

    ui->menuInformation->setTitle(tr("Information"));
    ui->menuInformation->actions().at(0)->setText(tr("&Reference"));

    actions.clear();
    actions = ui->menuOptions->actions();
    ui->menuOptions->setTitle(tr("Options"));
    actions.at(0)->setText(tr("&Switch language"));
    actions.at(1)->setText(tr("&Change shortcuts"));
    actions.at(2)->setText(tr("&Quit"));

    // tried some other methods to make its window translated
    // such as emitting signal, but in this case action names
    // didn't change, so this appeared to be the only one working
    QList<QMenu*> menus;
    menus.append(ui->menuFile);
    menus.append(ui->menuInformation);
    menus.append(ui->menuOptions);

    scChanger_ = new ShortcutChanger(nullptr, menus, &translator_);
}

void TextRedactor::createNewFile()
{
    if (!filename_.isEmpty())
    {
        save();
    }

    filename_ = "untitled.txt";
    this->setWindowTitle(filename_);
}

void TextRedactor::open()
{
    filename_ = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), filter_);
    QFile fileIn(filename_);
    if (fileIn.open(QIODevice::ReadOnly))
    {
        this->setWindowTitle(filename_);
        QByteArray data = fileIn.readAll();
        ui->textEdit->setText(data.data());
        ui->textEdit->setReadOnly(false);
    }
}

void TextRedactor::openReadOnly()
{
    filename_ = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), filter_);
    QFile fileIn(filename_);
    if (fileIn.open(QIODevice::ReadOnly))
    {
        this->setWindowTitle(filename_);
        QByteArray data = fileIn.readAll();
        ui->textEdit->setText(data.data());
        ui->textEdit->setReadOnly(true);
    }
}

void TextRedactor::save()
{
    if (!filename_.isEmpty() && "untitled.txt" != filename_)
    {
        QFile out(filename_);

        if (out.open(QIODevice::WriteOnly))
        {
            QString tmp = ui->textEdit->toPlainText();
            QByteArray data = tmp.toUtf8();
            out.write(data);
        }
    }
    else
    {
        saveAs();
    }
}

void TextRedactor::saveAs()
{
    QString tmp = ui->textEdit->toPlainText();
    QByteArray data = tmp.toUtf8();

    if (!filename_.isEmpty())
    {
        QFileDialog::saveFileContent(data, filename_);
    }
    else
    {
        QFileDialog::saveFileContent(data);
    }
}

void TextRedactor::openReference()
{
    QFile ref;
    if ("ru_RU" == translator_.language())
    {
        ref.setFileName(":/Resources/Reference_ru.txt");
    }
    else
    {
        ref.setFileName(":/Resources/Reference_en.txt");
    }

    if (ref.open(QIODevice::ReadOnly))
    {
        QByteArray data = ref.readAll();
        QMessageBox::information(this, tr("Reference"), QString(data));
    }
}

void TextRedactor::switchLanguage()
{
    if ("ru_RU" == translator_.language())
    {
        setLanguage("en");
    }
    else
    {
        setLanguage("ru");
    }
}

void TextRedactor::changeShortcuts()
{
    scChanger_->show();
}

// Probably it could have been done less complex, will try to update.
void TextRedactor::shortcutsChanged(QMap<QString, QKeySequence> &shortcuts)
{
    QList<QAction*> actions;
    actions.append(ui->menuFile->actions());
    actions.append(ui->menuInformation->actions());
    actions.append(ui->menuOptions->actions());

    for (quint16 i = 0; i < actions.size(); ++i)
    {
        if (shortcuts.contains(actions.at(i)->text()))
        {
            actions.at(i)->setShortcut(shortcuts[actions.at(i)->text()]);
        }
    }
}

void TextRedactor::quit()
{
    qApp->quit();
    return;
}
