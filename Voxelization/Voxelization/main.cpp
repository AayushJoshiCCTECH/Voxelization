#include "Vox.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Vox w;
    w.show();
    return a.exec();
}
