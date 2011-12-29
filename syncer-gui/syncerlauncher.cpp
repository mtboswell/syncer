#include "syncerlauncher.h"


SyncerLauncher::SyncerLauncher(QObject *parent) :
	QObject(parent)
{
	settings = new QSettings("MiBoSoft", "Syncer");
	trayIcon = new QSystemTrayIcon(QIcon(":/s_icon.svg"));

	procOutMapper = new QSignalMapper(this);

	connect(procOutMapper, SIGNAL(mapped(const QString &)), this, SLOT(readProcOut(const QString &)));

	//syncerPath = "../../../syncer/build/release/syncer.exe";
	if(settings->contains("syncerPath")){
		syncerPath = settings->value("syncerPath").toString();
	}else{
#ifdef Q_WS_WIN
		syncerPath = "syncer";
#else
		syncerPath = "syncer";
#endif
	}

	syncMenu = new QMenu;

	QAction* addAction = syncMenu->addAction("Add Directory");
	QAction* quitAction = syncMenu->addAction("Quit");
	syncMenu->addSeparator();

	connect(addAction, SIGNAL(triggered()), this, SLOT(addPath()));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quitAll()));

	menuMapper = new QSignalMapper();

	connect(menuMapper, SIGNAL(mapped(const QString &)),
					 this, SLOT(doAction(const QString &)));

	trayIcon->setContextMenu(syncMenu);

	QStringList dirs = settings->value("syncDirs").toStringList();

	foreach(QString dir, dirs){
		addPath(dir);
	}

	trayIcon->show();

}

void SyncerLauncher::addPath(){

	QString dir = QFileDialog::getExistingDirectory(0, "Open Git Repository Base Directory",
				QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	addPath(dir);

	QStringList dirs = settings->value("syncDirs").toStringList();

	dirs << dir;

	settings->setValue("syncDirs", dirs);
}

void SyncerLauncher::addPath(QString dir){

	syncDirs << dir;

	dirMenus[dir] = syncMenu->addMenu(dir);

	QAction* removeAction = dirMenus[dir]->addAction("Forget Directory");
	connect(removeAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(removeAction, "Remove:" + dir);

	QAction* restoreAction = dirMenus[dir]->addAction("Restore Files");
	connect(restoreAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(restoreAction, "Restore:" + dir);

	QAction* undeleteAction = dirMenus[dir]->addAction("Undelete Files");
	connect(undeleteAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(undeleteAction, "Undelete:" + dir);

	start(dir);
}

void SyncerLauncher::doAction(QString action){
	if(action.startsWith("Start:")) start(action.right(action.length()-6));
	else if(action.startsWith("Stop:")) stop(action.right(action.length()-5));
	else if(action.startsWith("Remove:")) remove(action.right(action.length()-7));
	else if(action.startsWith("Restore:")) restore(action.right(action.length()-8));
	else if(action.startsWith("Undelete:")) undelete(action.right(action.length()-9));
}

void SyncerLauncher::start(QString path){
	if(!syncers.contains(path)){
		syncers[path] = new QProcess();
		connect(syncers[path], SIGNAL(readyReadStandardOutput()), procOutMapper, SLOT(map()));
		procOutMapper->setMapping(syncers[path], path);
	}
	QStringList args;
	args << path;
	syncers[path]->start(syncerPath, args);
	if(!syncers[path]->waitForStarted()) qFatal("Error: syncer did not start");

	dirMenus[path]->removeAction(findMenuItem(dirMenus[path], "Start Syncing"));

	QAction* stopAction = dirMenus[path]->addAction("Stop Syncing");
	connect(stopAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(stopAction, "Stop:" + path);

}
void SyncerLauncher::stop(QString path){
	if(!syncers.contains(path)) syncers[path] = new QProcess();
	syncers[path]->kill();

	dirMenus[path]->removeAction(findMenuItem(dirMenus[path], "Stop Syncing"));

	QAction* startAction = dirMenus[path]->addAction("Start Syncing");
	connect(startAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(startAction, "Start:" + path);
}

void SyncerLauncher::remove(QString path){
	stop(path);
	syncMenu->removeAction(findMenuItem(syncMenu, path));

	QStringList dirs = settings->value("syncDirs").toStringList();

	dirs.removeAll(path);

	settings->setValue("syncDirs", dirs);
}


void SyncerLauncher::restore(QString path){
	QProcess* restoreProc = new QProcess();
	QStringList args;
	args << path;
	restoreProc->start("syncer-restore", args);
	if(!restoreProc->waitForStarted()) qWarning("Error: restore did not start");
}

void SyncerLauncher::undelete(QString path){
	QProcess* undeleteProc = new QProcess();
	QStringList args;
	args << path;
	undeleteProc->start("syncer-undelete", args);
	if(!undeleteProc->waitForStarted()) qWarning("Error: undelete did not start");
}





QAction* SyncerLauncher::findMenuItem(QMenu* menu, QString item){
	foreach(QAction* action, menu->actions()){
		if(action->text() == item) return action;
	}
	return new QAction("Dummy", this);
}

void SyncerLauncher::quitAll(){
	foreach(QString dir, syncDirs){
		stop(dir);
	}

	emit quit();
}

void SyncerLauncher::readProcOut(const QString & path){
	QByteArray procOut = syncers[path]->readAllStandardOutput();
	trayIcon->showMessage(procOut, path);
}
