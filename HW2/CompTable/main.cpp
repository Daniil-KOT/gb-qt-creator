#include "comptable.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CompTable w;
    w.show();
    return a.exec();
}
