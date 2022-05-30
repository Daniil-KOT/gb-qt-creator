#include "langstable.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LangsTable w;
    w.show();
    return a.exec();
}
