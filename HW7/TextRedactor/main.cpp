#include "textredactor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextRedactor w;
    w.show();
    return a.exec();
}
