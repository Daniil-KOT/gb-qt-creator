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

    QAction* actOpen = new QAction(QIcon(":/Resources/icons/open.png"), "&Open", this);
    QAction* actSave = new QAction(QIcon(":/Resources/icons/save.png"), "&Save", this);
    QAction* actSaveAs = new QAction(QIcon(":/Resources/icons/save_as.png"), "&Save as", this);
    QAction* actReference = new QAction(QIcon(":/Resources/icons/info.png"), "&Reference", this);

    actOpen->setShortcut(QKeySequence::Open);
    actSave->setShortcut(QKeySequence::Save);

#ifdef Q_OS_WIN

    actSaveAs->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);

#elif Q_OS_UNIX

    // does not work with KDE Plasma
    actSaveAs->setShortcut(QKeySequence::SaveAs);

#endif

    actReference->setShortcut(QKeySequence::HelpContents);

    ui->menuFile->addAction(actOpen);
    ui->menuFile->addAction(actSave);
    ui->menuFile->addAction(actSaveAs);
    ui->menuInformation->addAction(actReference);

    connect(actOpen, &QAction::triggered, this, &TextRedactor::open);
    connect(actSave, &QAction::triggered, this, &TextRedactor::save);
    connect(actSaveAs, &QAction::triggered, this, &TextRedactor::saveAs);
    connect(actReference, &QAction::triggered, this, &TextRedactor::openReference);
}

TextRedactor::~TextRedactor()
{
    delete ui;
}

void TextRedactor::open()
{
    filename_ = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), filter_);
    QFile fileIn(filename_);
    if (fileIn.open(QIODevice::ReadOnly))
    {
        this->setWindowTitle(filename_);
        QByteArray data = fileIn.readAll();
        ui->textEdit->setText(data.data());
    }
}

void TextRedactor::save()
{
    if (!filename_.isEmpty())
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
    QFile ref(":/Resources/Reference.txt");

    if (ref.open(QIODevice::ReadOnly))
    {
        QByteArray data = ref.readAll();
        QMessageBox::information(this, "Reference", QString(data));
    }
}

