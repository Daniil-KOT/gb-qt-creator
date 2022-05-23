#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_AppToLeft_clicked();

    void on_AppToRight_clicked();

    void on_ReplaceLeft_clicked();

    void on_ReplaceRight_clicked();

    void on_HtmlToLeft_clicked();

    void on_HtmlToRight_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QString> codes_;
};
#endif // MAINWINDOW_H
