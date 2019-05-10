#include "TestMain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CTestMain w;
    w.show();

    return a.exec();
}
