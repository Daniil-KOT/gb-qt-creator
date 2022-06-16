#include "basicshapes.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BasicShapes w;
    w.show();
    return a.exec();
}
