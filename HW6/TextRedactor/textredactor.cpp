#include "textredactor.h"
#include "ui_textredactor.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QAction>
#include <QKeySequence>
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QMdiSubWindow>

TextRedactor::TextRedactor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextRedactor)
    , filename_("")
    , filter_("Text documents(*.txt)")
{
    ui->setupUi(this);

    mdiArea_ = new QMdiArea(this);
    ui->gridLayout->addWidget(mdiArea_, 1, 0, 10, 1);

    QMdiSubWindow* initial = mdiArea_->addSubWindow(new CustomTextEdit(this, &translator_));
    initial->setWindowState(Qt::WindowState::WindowMaximized);
    createNewFile(false);

    this->setWindowTitle("Simple Text Redactor v0.1");
    this->setWindowIcon(QIcon(":/Resources/icons/mainwindow.png"));

    // menu File actions
    QAction* actCreateNew = new QAction(QIcon(":/Resources/icons/new_file.png"), tr("New file"), this);
    QAction* actCreateSeparated = new QAction(QIcon(":/Resources/icons/new_file.png"), tr("New file in separate editor"), this);
    QAction* actOpen = new QAction(QIcon(":/Resources/icons/open.png"), tr("Open"), this);
    QAction* actOpenSeparated = new QAction(QIcon(":/Resources/icons/open.png"), tr("Open in separate editor"), this);
    QAction* actOpenReadOnly = new QAction(QIcon(":Resources/icons/open.png"), tr("Open read only"), this);
    QAction* actSave = new QAction(QIcon(":/Resources/icons/save.png"), tr("Save"), this);
    QAction* actSaveAs = new QAction(QIcon(":/Resources/icons/save_as.png"), tr("Save as"), this);
    QAction* actPrint = new QAction(QIcon(":/Resources/icons/print.png"), tr("Print"), this);

    // menu Information actions
    QAction* actReference = new QAction(QIcon(":/Resources/icons/info.png"), tr("Reference"), this);

    // menu Options actions
    QAction* actSwitchLang = new QAction(QIcon(":/Resources/icons/switch_lang.png"), tr("Switch language"), this);
    QAction* actChangeShortcuts = new QAction(QIcon(":/Resources/icons/chng_shortcuts.png"), tr("Change shortcuts"), this);
    QAction* actQuit = new QAction(QIcon(":/Resources/icons/quit.png"), tr("Quit"), this);

    actCreateNew->setShortcut(QKeySequence::New);
    actCreateSeparated->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_N);
    actOpen->setShortcut(QKeySequence::Open);
    actOpenSeparated->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_I);
    actOpenReadOnly->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_O);
    actSave->setShortcut(QKeySequence::Save);
    actPrint->setShortcut(QKeySequence::Print);

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
    ui->menuFile->addAction(actCreateSeparated);
    ui->menuFile->addAction(actOpen);
    ui->menuFile->addAction(actOpenSeparated);
    ui->menuFile->addAction(actOpenReadOnly);
    ui->menuFile->addAction(actSave);
    ui->menuFile->addAction(actSaveAs);
    ui->menuFile->addAction(actPrint);

    ui->menuInformation->addAction(actReference);

    ui->menuOptions->addAction(actSwitchLang);
    ui->menuOptions->addAction(actChangeShortcuts);
    ui->menuOptions->addAction(actQuit);

    themes_ = TextRedactor::Themes(new QMenu(tr("Themes")),
                                   new QAction(tr("Dark")),
                                   new QAction(tr("Light")),
                                   new QAction("Fibers"),
                                   new QAction("Ubuntu"),
                                   new QAction(tr("Cyan")));

    ui->menuOptions->addMenu(themes_.menu_);

    QList<QMenu*> menus;
    menus.append(ui->menuFile);
    menus.append(ui->menuInformation);
    menus.append(ui->menuOptions);

    scChanger_ = new ShortcutChanger(nullptr, menus, &translator_);

    setLanguage("en");

    // menu File actions connects
    connect(actCreateNew, &QAction::triggered, this, &TextRedactor::createNewFile);
    connect(actCreateSeparated, &QAction::triggered, this, &TextRedactor::createNewFileSeparated);
    connect(actOpen, &QAction::triggered, this, &TextRedactor::open);
    connect(actOpenReadOnly, &QAction::triggered, this, &TextRedactor::openReadOnly);
    connect(actSave, &QAction::triggered, this, &TextRedactor::save);
    connect(actSaveAs, &QAction::triggered, this, &TextRedactor::saveAs);
    connect(actPrint, &QAction::triggered, this, &TextRedactor::print);

    // menu Information actions connects
    connect(actReference, &QAction::triggered, this, &TextRedactor::openReference);

    // menu Options actions connects
    connect(actSwitchLang, &QAction::triggered, this, &TextRedactor::switchLanguage);
    connect(actChangeShortcuts, &QAction::triggered, this, &TextRedactor::changeShortcuts);
    connect(scChanger_, &ShortcutChanger::shortcutsChanged, this, &TextRedactor::shortcutsChanged);
    connect(actQuit, &QAction::triggered, this, &TextRedactor::quit);

    // menu Themes actions connects
    connect(themes_.actSwitchDark, &QAction::triggered, this, &TextRedactor::switchTheme);
    connect(themes_.actSwitchLight, &QAction::triggered, this, &TextRedactor::switchTheme);
    connect(themes_.actSwitchFibers, &QAction::triggered, this, &TextRedactor::switchTheme);
    connect(themes_.actSwitchUbuntu, &QAction::triggered, this, &TextRedactor::switchTheme);
    connect(themes_.actSwitchCyan, &QAction::triggered, this, &TextRedactor::switchTheme);

    connect(mdiArea_, &QMdiArea::subWindowActivated, this, &TextRedactor::currentTEChanged);

    // setting Light theme by default
    emit themes_.actSwitchLight->triggered();
}

TextRedactor::~TextRedactor()
{
    delete ui;
}

void TextRedactor::setLanguage(QString lang)
{
    if (translator_.load(":/Textredactor_" + lang))
    {
        qApp->installTranslator(&translator_);

        if (scChanger_)
        {
            if (scChanger_->isEnabled())
                scChanger_->close();

            delete scChanger_;
            scChanger_ = nullptr;
        }

        currentTextEdit_->updateTranslator();
    }

    ui->menuFile->setTitle(tr("File"));
    ui->menuInformation->setTitle(tr("Information"));
    ui->menuOptions->setTitle(tr("Options"));
    themes_.menu_->setTitle(tr("Themes"));

    QList<QAction*> actions;
    actions.append(ui->menuFile->actions());
    actions.append(ui->menuInformation->actions());
    actions.append(ui->menuOptions->actions());
    actions.append(themes_.menu_->actions());

    for (QAction* action : actions)
    {
        action->setText(tr(action->text().toStdString().c_str()));
    }

    // tried some other methods to make its window translated
    // such as emitting signal, but in this case action names
    // didn't change, so this appeared to be the only one working
    QList<QMenu*> menus;
    menus.append(ui->menuFile);
    menus.append(ui->menuInformation);
    menus.append(ui->menuOptions);

    scChanger_ = new ShortcutChanger(nullptr, menus, &translator_);
}

void TextRedactor::createNewFile(bool separated)
{
    if (currentTextEdit_ && !currentTextEdit_->toPlainText().isEmpty() && !separated)
    {
        QMessageBox::information(this, tr("Notification"), tr("Don't forget to save file!"));
        saveAs();
    }

    filename_ = "untitled.txt";
    mdiArea_->currentSubWindow()->setWindowTitle(filename_);
}

void TextRedactor::createNewFileSeparated()
{
    mdiArea_->currentSubWindow()->setWindowState(Qt::WindowState::WindowNoState);
    mdiArea_->addSubWindow(new CustomTextEdit(this, &translator_))->show();
    createNewFile(true);
}

void TextRedactor::open()
{
    if (currentTextEdit_ && !currentTextEdit_->toPlainText().isEmpty())
    {
        QMessageBox::information(this, tr("Notification"), tr("Don't forget to save file!"));
        saveAs();
    }

    filename_ = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), filter_);
    QFile fileIn(filename_);

    if (fileIn.open(QIODevice::ReadOnly))
    {
        this->setWindowTitle(filename_);
        const QByteArray data = fileIn.readAll();
        currentTextEdit_->setText(data.data());
        currentTextEdit_->setReadOnly(false);
    }
}

void TextRedactor::openSeparated()
{
    if (currentTextEdit_ && !currentTextEdit_->toPlainText().isEmpty())
    {
        QMessageBox::information(this, tr("Notification"), tr("Don't forget to save file!"));
        saveAs();
    }

    mdiArea_->currentSubWindow()->setWindowState(Qt::WindowState::WindowNoState);
    mdiArea_->addSubWindow(new CustomTextEdit(this, &translator_))->show();
    open();
}

void TextRedactor::openReadOnly()
{
    if (currentTextEdit_ && !currentTextEdit_->toPlainText().isEmpty())
    {
        QMessageBox::information(this, tr("Notification"), tr("Don't forget to save file!"));
        saveAs();
    }

    filename_ = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), filter_);
    QFile fileIn(filename_);
    if (fileIn.open(QIODevice::ReadOnly))
    {
        this->setWindowTitle(filename_);
        const QByteArray data = fileIn.readAll();
        currentTextEdit_->setText(data.data());
        currentTextEdit_->setReadOnly(true);
    }
}

void TextRedactor::save()
{
    if (!filename_.isEmpty() && "untitled.txt" != filename_)
    {
        QFile out(filename_);

        if (out.open(QIODevice::WriteOnly))
        {
            const QString tmp = currentTextEdit_->toPlainText();
            const QByteArray data = tmp.toUtf8();
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
    const QString tmp =  currentTextEdit_->toPlainText();
    const QByteArray data = tmp.toUtf8();
    QFileDialog::saveFileContent(data, filename_);
}

void TextRedactor::print()
{
    QPrinter printer;
    QPrintDialog printDlg(&printer, this);
    printDlg.setWindowTitle(tr("Print"));

    if (printDlg.exec() != QDialog::Accepted)
    {
        return;
    }

    currentTextEdit_->print(&printer);
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
        const QByteArray data = ref.readAll();
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
}

void TextRedactor::switchTheme()
{
    const QObject* obj = sender();
    QFile style;

    // DARK AND LIGHT STYLESHEETS TAKEN FROM:
    // https://github.com/ColinDuquesnoy/QDarkStyleSheet
    if (obj == themes_.actSwitchDark)
    {
        style.setFileName(":/Resources/styles/dark_theme.qss");
    }
    else if (obj == themes_.actSwitchLight)
    {
        style.setFileName(":/Resources/styles/light_theme.qss");
    }
    else if (obj == themes_.actSwitchFibers)
    {
        style.setFileName(":/Resources/styles/fibers.qss");

    }
    else if (obj == themes_.actSwitchUbuntu)
    {
        style.setFileName(":/Resources/styles/ubuntu.qss");

    }
    else if (obj == themes_.actSwitchCyan)
    {
        // created with QSS stylesheet editor:
        // https://github.com/hustlei/QssStylesheetEditor
        style.setFileName(":/Resources/styles/cyan_handmade.qss");
    }

    if (style.open(QIODevice::ReadOnly))
    {
        qApp->setStyleSheet(style.readAll());
    }
}

void TextRedactor::currentTEChanged(QMdiSubWindow *window)
{
    if (!window)
    {
        delete currentTextEdit_;
        currentTextEdit_ = nullptr;
        return;
    }
    currentTextEdit_ = (CustomTextEdit*)window->widget();
    currentTextEdit_->updateTranslator();
}
