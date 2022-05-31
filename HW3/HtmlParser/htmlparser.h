#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QMainWindow>

#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class HtmlParser; }
QT_END_NAMESPACE

class HtmlParser : public QMainWindow
{
    Q_OBJECT

public:
    HtmlParser(QWidget *parent = nullptr);
    ~HtmlParser();

private slots:
    void open();

    void on_showWeatherBtn_clicked();

    void on_showRatesBtn_clicked();

private:
    Ui::HtmlParser *ui;
    QString filter_;
    QFile file_;
};
#endif // HTMLPARSER_H
