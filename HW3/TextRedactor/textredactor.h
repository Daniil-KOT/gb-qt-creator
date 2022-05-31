#ifndef TEXTREDACTOR_H
#define TEXTREDACTOR_H

#include <QMainWindow>

#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class TextRedactor; }
QT_END_NAMESPACE

class TextRedactor : public QMainWindow
{
    Q_OBJECT

public:
    TextRedactor(QWidget *parent = nullptr);
    ~TextRedactor();

private slots:
    void open();

    void save();

    void saveAs();

    void openReference();

private:
    Ui::TextRedactor *ui;
    QString filename_;
    QString filter_;
};
#endif // TEXTREDACTOR_H
