#include "calcmainwindow.h"
#include "ui_calcmainwindow.h"
#include <QMessageBox>
#include <QtMath>

CalcMainWindow::CalcMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalcMainWindow)
{
    ui->setupUi(this);
    isDegrees_ = true;
}

CalcMainWindow::~CalcMainWindow()
{
    delete ui;
}

void CalcMainWindow::on_calculate_clicked()
{
    int a, b, c, x1, x2, disc;

    if (!ui->a_val->text().isEmpty())
        a = ui->a_val->text().toInt();
    else
        a = 1;

    if (!ui->b_val->text().isEmpty())
        b = ui->b_val->text().toInt();
    else
        b = 1;

    if (!ui->c_val->text().isEmpty())
        c = ui->c_val->text().toInt();
    else
        c = 1;

    if (b == 0)
    {
        QMessageBox::information(this, "Error", "b cannot equal to 0!");
        return;
    }

    if (a == 0)
    {
        if (b == 1)
        {
            QMessageBox::information(this, "Result", "x = " + QString::number(-c));
            return;
        }
        else
        {
            x1 = -c / b;
            QMessageBox::information(this, "Result", "x = " + QString::number(x1));
            return;
        }
    }

    disc = qPow(b, 2) - 4 * a * c;

    if (disc > 0)
    {
        x1 = (-b + qSqrt(disc)) / 2 * a;
        x2 = (-b - qSqrt(disc)) / 2 * a;
        QMessageBox::information(this, "Result", "x1 = " + QString::number(x1) + "\nx2 = " + QString::number(x2));
    }
    else if (disc == 0)
    {
        x1 = (-b + qSqrt(disc)) / 2 * a;
        QMessageBox::information(this, "Result", "x = " + QString::number(x1));
    }
    else
    {
        QMessageBox::information(this, "Result", "Equation does not have any root!");
    }
}

void CalcMainWindow::on_calculate_angle_clicked()
{
    int a, b, angle, res;

    if (!ui->side_a->text().isEmpty())
    {
        a = ui->side_a->text().toInt();
    }
    else
    {
        QMessageBox::information(this, "Warning", "Field 'a' cannot be empty!");
        return;
    }

    if (!ui->side_b->text().isEmpty())
    {
        b = ui->side_b->text().toInt();
    }
    else
    {
        QMessageBox::information(this, "Warning", "Field 'b' cannot be empty!");
        return;
    }

    if (!ui->angle_val->text().isEmpty())
    {
        if (isDegrees_)
            angle = ui->angle_val->text().toInt();
        else
            angle = qRadiansToDegrees(ui->angle_val->text().toDouble());
    }
    else
    {
        QMessageBox::information(this, "Warning", "Field 'b' cannot be empty!");
        return;
    }

    res = qSqrt(qPow(a, 2) + qPow(b, 2) - 2 * a * b * qCos(angle));

    QMessageBox::information(this, "Result", "Side = " + QString::number(res));
}

void CalcMainWindow::on_degrees_toggled(bool checked)
{
    isDegrees_ = true;
}


void CalcMainWindow::on_radians_toggled(bool checked)
{
    isDegrees_ = false;
}

