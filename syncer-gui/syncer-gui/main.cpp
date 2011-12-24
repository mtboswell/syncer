#include <QApplication>
#include "syncerlauncher.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Q_INIT_RESOURCE(icons);
	a.setQuitOnLastWindowClosed(false);

	SyncerLauncher* syncerLauncher = new SyncerLauncher;

	QObject::connect(syncerLauncher, SIGNAL(quit()), &a, SLOT(quit()));


    return a.exec();
}
