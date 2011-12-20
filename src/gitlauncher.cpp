#include "gitlauncher.h"
#include <QDebug>

GitLauncher::GitLauncher(){

}

void GitLauncher::fileChanged(QString path){
	qDebug() << "File Changed:" << path;

}
void GitLauncher::directoryChanged(QString path){
	qDebug() << "Directory Changed:" << path;

}
void GitLauncher::checkForUpdate(){

}
