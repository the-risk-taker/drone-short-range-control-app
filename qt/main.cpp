#include "DroneShortRangeComm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DroneShortRangeComm w;
    w.show();

    return a.exec();
}
