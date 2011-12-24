#include "syncerlauncher.h"
#include <QDebug>

SyncerLauncher::SyncerLauncher(QObject *parent) :
	QObject(parent)
{
	syncerPath = "../../syncer/build/release/syncer.exe";
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
	syncers[path]->terminate();
}
void SyncerLauncher::remove(QString path){

}
