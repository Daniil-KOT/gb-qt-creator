#ifndef CALCMAINWINDOW_H
#define CALCMAINWINDOW_H

#include <QMainWindow>
#include "quadraticcalc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CalcMainWindow; }
QT_END_NAMESPACE

class CalcMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CalcMainWindow(QWidget *parent = nullptr);
    ~CalcMainWindow();

private slots:
    void on_calculate_clicked();

    void on_calculate_angle_clicked();

    void on_degrees_toggled(bool checked);

    void on_radians_toggled(bool checked);

private:
    Ui::CalcMainWindow *ui;
    bool isDegrees_;
};
#endif // CALCMAINWINDOW_H
