#include "htmlparser.h"
#include "ui_htmlparser.h"

#include <QRegularExpression>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

HtmlParser::HtmlParser(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HtmlParser)
    , filter_("HTML file(*.html)")
{
    ui->setupUi(this);
    QAction* actOpen = new QAction("&Open", this);
    actOpen->setShortcut(QKeySequence::Open);

    ui->menuMenu->addAction(actOpen);

    connect(actOpen, &QAction::triggered, this, &HtmlParser::open);

}

HtmlParser::~HtmlParser()
{
    delete ui;
}

void HtmlParser::open()
{
    QString filename_ = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), filter_);
    file_.setFileName(filename_);
}


void HtmlParser::on_showWeatherBtn_clicked()
{
    if (file_.fileName().isEmpty())
    {
        QMessageBox::information(this, "WARNING", "Please open file first!");
        return;
    }

    QRegularExpression regExpWeather("<div class=\"weather svelte-likh3p.*<div class=\"rates svelte-1ydy7z3");

    if (file_.open(QIODevice::ReadOnly))
    {
        QString tmp = QString(file_.readAll());
        QRegularExpressionMatch match = regExpWeather.match(tmp);

        if (match.hasMatch())
        {
            ui->textEdit->clear();
            ui->textEdit->setText(match.captured().remove("<div class=\"rates svelte-1ydy7z3"));
        }
        file_.close();
    }
}


void HtmlParser::on_showRatesBtn_clicked()
{
    if (file_.fileName().isEmpty())
    {
        QMessageBox::information(this, "WARNING", "Please open file first!");
        return;
    }

    QRegularExpression regExpRates("<div class=\"rates svelte-1ydy7z3.*<a class=\"horoscope i-link-no-deco");

    if (file_.open(QIODevice::ReadOnly))
    {
        QString tmp = QString(file_.readAll());
        auto match = regExpRates.match(tmp);

        if (match.hasMatch())
        {
            ui->textEdit->clear();
            ui->textEdit->setText(match.captured().remove("<a class=\"horoscope i-link-no-deco"));
        }
        file_.close();
    }
}

