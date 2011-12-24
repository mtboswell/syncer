#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include "syncerlauncher.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	SyncerLauncher* syncerLauncher = new SyncerLauncher;


	QSystemTrayIcon* trayIcon = new QSystemTrayIcon(QIcon("s_icon.svg"));
	trayIcon->show();


	QStringList syncDirs;

	syncDirs << "C:\\Users\\micah\\Documents\\working\\syncer\\";

	QMenu syncMenu;

	syncMenu.addAction("Add Directory");
	syncMenu.addSeparator();

	QSignalMapper* menuMapper = new QSignalMapper();

	foreach(QString dir, syncDirs){
		QMenu* dirMenu = syncMenu.addMenu(dir);

		QAction* startAction = dirMenu->addAction("Start Syncing");
		QObject::connect(startAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
		menuMapper->setMapping(startAction, "Start:" + dir);

		QAction* stopAction = dirMenu->addAction("Stop Syncing");
		QObject::connect(stopAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
		menuMapper->setMapping(stopAction, "Stop:" + dir);

		QAction* removeAction = dirMenu->addAction("Remove Direcctory");
		QObject::connect(removeAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
		menuMapper->setMapping(removeAction, "Remove:" + dir);
	}

	QObject::connect(menuMapper, SIGNAL(mapped(const QString &)),
					 syncerLauncher, SLOT(doAction(const QString &)));

	trayIcon->setContextMenu(&syncMenu);

    return a.exec();
}
