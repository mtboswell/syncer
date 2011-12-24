#include <QtGui/QApplication>
#include "syncergui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SyncerGui w;
    w.show();
    
    return a.exec();
}
