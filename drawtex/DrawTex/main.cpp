#include "drawtex.h"
#include <QApplication>
#include <QString>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DrawTex w;
    w.show();

    return a.exec();
}
