#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand(time(0));

    codes_.push_back("<h2 title=\"I'm a header\">The title Attribute</h2>");
    codes_.push_back("<h1 style=\"color:blue;\">This is a heading</h1>");
    codes_.push_back("<p>This text is normal.</p>");
    codes_.push_back("<p>This is <sub>subscripted</sub> text.</p>");
    codes_.push_back("<p><b>This text is bold.</b></p>");
    codes_.push_back("<p><strong>This text is important!</strong></p>");
    codes_.push_back("<p><i>This text is italic.</i></p>");
    codes_.push_back("<p><em>This text is emphasized.</em></p>");
    codes_.push_back("<p><small>This is some smaller text.</small></p>");
    codes_.push_back("<p>This is <sup>superscripted</sup> text.</p>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AppToLeft_clicked()
{
    if (!ui->rightText->toPlainText().isEmpty())
    {
        ui->leftText->appendPlainText(ui->rightText->toPlainText());
    }
    else
    {
        QMessageBox::information(this, "Warning", "Empty editor!");
    }
}

void MainWindow::on_AppToRight_clicked()
{
    if (!ui->leftText->toPlainText().isEmpty())
    {
        ui->rightText->appendPlainText(ui->leftText->toPlainText());
    }
    else
    {
        QMessageBox::information(this, "Warning", "Empty editor!");
    }
}

void MainWindow::on_ReplaceLeft_clicked()
{
    if (!ui->rightText->toPlainText().isEmpty())
    {
        ui->leftText->clear();
        ui->leftText->appendPlainText(ui->rightText->toPlainText());
    }
    else
    {
        QMessageBox::information(this, "Warning", "Empty editor!");
    }
}

void MainWindow::on_ReplaceRight_clicked()
{
    if (!ui->leftText->toPlainText().isEmpty())
    {
        ui->rightText->clear();
        ui->rightText->appendPlainText(ui->leftText->toPlainText());
    }
    else
    {
        QMessageBox::information(this, "Warning", "Empty editor!");
    }
}


void MainWindow::on_HtmlToLeft_clicked()
{
    ui->leftText->appendHtml(codes_.at(rand() % 10));
}


void MainWindow::on_HtmlToRight_clicked()
{
    ui->rightText->appendHtml(codes_.at(rand() % 10));
}

