#include <QApplication>

#include "formplayerslider.h"
#include "zplayerslider.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FormPlayerSlider w;
    w.show();

    return a.exec();
}
