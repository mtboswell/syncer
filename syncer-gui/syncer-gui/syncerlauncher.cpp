#include "syncerlauncher.h"


SyncerLauncher::SyncerLauncher(QObject *parent) :
	QObject(parent)
{
	syncerPath = "../../../syncer/build/release/syncer.exe";
	trayIcon = new QSystemTrayIcon(QIcon("s_icon.svg"));
	trayIcon->show();

	syncMenu = new QMenu;

	QAction* addAction = syncMenu->addAction("Add Directory");
	QAction* quitAction = syncMenu->addAction("Quit");
	syncMenu->addSeparator();

	connect(addAction, SIGNAL(triggered()), this, SLOT(addPath()));
	connect(quitAction, SIGNAL(triggered()), this, SIGNAL(quit()));

	menuMapper = new QSignalMapper();

	connect(menuMapper, SIGNAL(mapped(const QString &)),
					 this, SLOT(doAction(const QString &)));

	trayIcon->setContextMenu(syncMenu);
}

void SyncerLauncher::addPath(){

	QString dir = QFileDialog::getExistingDirectory(0, "Open Git Repository Base Directory",
				QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	syncDirs << dir;

	dirMenus[dir] = syncMenu->addMenu(dir);

	QAction* startAction = dirMenus[dir]->addAction("Start Syncing");
	connect(startAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(startAction, "Start:" + dir);

	QAction* stopAction = dirMenus[dir]->addAction("Stop Syncing");
	connect(stopAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(stopAction, "Stop:" + dir);

	QAction* removeAction = dirMenus[dir]->addAction("Forget Direcctory");
	connect(removeAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(removeAction, "Remove:" + dir);
}

void SyncerLauncher::doAction(QString action){
	if(action.startsWith("Start:")) start(action.right(action.length()-6));
	else if(action.startsWith("Stop:")) stop(action.right(action.length()-5));
	else if(action.startsWith("Remove:")) remove(action.right(action.length()-7));
}

void SyncerLauncher::start(QString path){
	if(!syncers.contains(path)) syncers[path] = new QProcess();
	QStringList args;
	args << path;
	syncers[path]->start(syncerPath, args);
	if(!syncers[path]->waitForStarted()) qDebug() << "Error: syncer did not start";
}
void SyncerLauncher::stop(QString path){
	if(!syncers.contains(path)) syncers[path] = new QProcess();
	syncers[path]->kill();
}
void SyncerLauncher::remove(QString path){
	stop(path);
	syncMenu->removeAction(findMenuItem(syncMenu, path));
	delete dirMenus[path];
}

QAction* SyncerLauncher::findMenuItem(QMenu* menu, QString item){
	foreach(QAction* action, menu->actions()){
		if(action->text() == item) return action;
	}
	return new QAction("Dummy", this);
}
