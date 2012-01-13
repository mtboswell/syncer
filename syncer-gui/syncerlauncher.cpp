#include "syncerlauncher.h"


SyncerLauncher::SyncerLauncher(QObject *parent) :
	QObject(parent)
{
	settings = new QSettings("MiBoSoft", "Syncer");
	trayIcon = new QSystemTrayIcon(QIcon(":/s_icon.svg"));

	procOutMapper = new QSignalMapper(this);
	procFinishMapper = new QSignalMapper(this);

	connect(procOutMapper, SIGNAL(mapped(const QString &)), this, SLOT(readProcOut(const QString &)));
	connect(procFinishMapper, SIGNAL(mapped(const QString &)), this, SLOT(syncerCrashed(const QString &)));

	//syncerPath = "../../../syncer/build/release/syncer.exe";
	// todo: use app->appPath(), etc.
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

	QAction* initAction = syncMenu->addAction("Add Directory From Server");
	QAction* addAction = syncMenu->addAction("Re-Add Forgotten Directory");
	QAction* quitAction = syncMenu->addAction("Quit");
	syncMenu->addSeparator();

	connect(initAction, SIGNAL(triggered()), this, SLOT(initPath()));
	connect(addAction, SIGNAL(triggered()), this, SLOT(addPath()));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quitAll()));

	menuMapper = new QSignalMapper();

	connect(menuMapper, SIGNAL(mapped(const QString &)),
					 this, SLOT(doAction(const QString &)));

	trayIcon->setContextMenu(syncMenu);

	refreshMenu();

	trayIcon->show();

}

void SyncerLauncher::refreshMenu(){
	foreach(QString path, syncDirs){
		stop(path);
		syncMenu->removeAction(findMenuItem(syncMenu, QDir::toNativeSeparators(path)));
	}

	QStringList dirs = settings->value("syncDirs").toStringList();

	foreach(QString dir, dirs){
		addPath(dir);
	}
}

void SyncerLauncher::initPath(){
	Init* init = new Init;
	connect(init, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(refreshMenu()));
	connect(init, SIGNAL(finished(int, QProcess::ExitStatus)), init, SLOT(deleteLater()));
	init->show();
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

	dirMenus[dir] = syncMenu->addMenu(QDir::toNativeSeparators(dir));

	QAction* removeAction = dirMenus[dir]->addAction("Forget Directory");
	connect(removeAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(removeAction, "Remove:" + dir);

	QAction* restoreAction = dirMenus[dir]->addAction("Restore Files from earlier versions");
	connect(restoreAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(restoreAction, "Restore:" + dir);

	QAction* undeleteAction = dirMenus[dir]->addAction("Undelete Files");
	connect(undeleteAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(undeleteAction, "Undelete:" + dir);

	QAction* shredAction = dirMenus[dir]->addAction("Permenantly Delete Files");
	connect(shredAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(shredAction, "Shred:" + dir);

	start(dir);
}

void SyncerLauncher::doAction(QString action){
	if(action.startsWith("Start:")) start(action.right(action.length()-6));
	else if(action.startsWith("Stop:")) stop(action.right(action.length()-5));
	else if(action.startsWith("Remove:")) remove(action.right(action.length()-7));
	else if(action.startsWith("Restore:")) restore(action.right(action.length()-8));
	else if(action.startsWith("Undelete:")) undelete(action.right(action.length()-9));
	else if(action.startsWith("Shred:")) shred(action.right(action.length()-6));
}

void SyncerLauncher::start(QString path){
	if(!syncers.contains(path)){
		syncers[path] = new QProcess();
		connect(syncers[path], SIGNAL(readyReadStandardOutput()), procOutMapper, SLOT(map()));
		connect(syncers[path], SIGNAL(finished(int, QProcess::ExitStatus)), procFinishMapper, SLOT(map()));
		procOutMapper->setMapping(syncers[path], path);
		procFinishMapper->setMapping(syncers[path], path);
	}
	QStringList args;
	args << path;
	syncers[path]->start(syncerPath, args);
	if(!syncers[path]->waitForStarted()){
		trayIcon->showMessage("Error", "Syncer did not start!");
		//qFatal("Error: syncer did not start");
	}

	dirMenus[path]->removeAction(findMenuItem(dirMenus[path], "Start Syncing"));

	QAction* stopAction = dirMenus[path]->addAction("Stop Syncing");
	connect(stopAction, SIGNAL(triggered()), menuMapper, SLOT(map()));
	menuMapper->setMapping(stopAction, "Stop:" + path);

}
void SyncerLauncher::stop(QString path){
	if(!syncers.contains(path)) syncers[path] = new QProcess();
	procsKilled << path;
	syncers[path]->kill();
	syncers[path]->waitForFinished();

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
	if(!QProcess::startDetached("syncer-restore", QStringList() << path)) QMessageBox::critical(0, "Error", "Error: restore did not start");
}

void SyncerLauncher::undelete(QString path){
	QProcess* undeleteProc = new QProcess();
	QStringList args;
	args << path;
	undeleteProc->start("syncer-undelete", args);
	if(!undeleteProc->waitForStarted()) qWarning("Error: undelete did not start");
}

void SyncerLauncher::shred(QString path){
	QProcess* shredProc = new QProcess();
	QStringList args;
	args << path;
	shredProc->start("syncer-shred", args);
	if(!shredProc->waitForStarted()) qWarning("Error: shred did not start");
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
	trayIcon->showMessage(path, procOut);
}

void SyncerLauncher::syncerCrashed(const QString & path){
	if(procsKilled.contains(path)){
		procsKilled.removeAll(path);
		return;
	}

	QStringList args;
	args << path;
	syncers[path]->start(syncerPath, args);
	if(!syncers[path]->waitForStarted()){
		trayIcon->showMessage("Error", "Syncer did not restart!");
		//qFatal("Error: syncer did not start");
	}
}
