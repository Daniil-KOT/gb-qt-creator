#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , parser_(new SpecSyms)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (parser_)
        delete parser_;
    delete ui;
}


void MainWindow::on_plainTextEdit_textChanged()
{
    QString text = ui->plainTextEdit->toPlainText();
    if (parser_->changed(text))
    {
        ui->plainTextEdit->setPlainText(parser_->getText());

        // moving cursor at the end of the text
        auto cursor = ui->plainTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->plainTextEdit->setTextCursor(cursor);
    }
}

