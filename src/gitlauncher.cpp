#include "gitlauncher.h"
#include <QDebug>

GitLauncher::GitLauncher(){
	git = "git";
	gitproc = new QProcess();
	pushTimer = new QTimer();
	connect(pushTimer, SIGNAL(timeout()), this, SLOT(doPush()));
}

void GitLauncher::fileChanged(QString path){
	qDebug() << "File Changed:" << path;

}
void GitLauncher::directoryChanged(QString path){
	qDebug() << "Directory Changed:" << path;
	if(!dirsChanged.contains(path))
		dirsChanged << path;
	pushTimer->start(5000);

}

void GitLauncher::checkForUpdate(){
	// git pull
	
	QStringList pullArgs;
	pullArgs << "pull";

	gitproc->start(git, pullArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	gitOut = gitproc->readAll();

	qDebug() << "Git pull output:" << gitOut;

}

void GitLauncher::doPush(){
	pushTimer->stop();
	// git add each dir 
	foreach(QString path, dirsChanged){
		gitproc->setWorkingDirectory(path);

		// git add dir

		QStringList addArgs;
		addArgs << "add" << "--all";

		gitproc->start(git, addArgs);
		if(!gitproc->waitForStarted()) return;
		if(!gitproc->waitForFinished()) return;
		gitOut = gitproc->readAll();

		qDebug() << "Git add output:" << gitOut;
	}
	dirsChanged.clear();



	// git commit
	// note that the commit happens in the last dir that git add was run in. This means we don't support multiple repos per process.

	QStringList commitArgs;
	commitArgs << "commit" << "-m" << "\"Autosync commit\"";
	qDebug() << "Commit args:" << commitArgs;

	gitproc->start(git, commitArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	gitOut = gitproc->readAll();

	qDebug() << "Git commit output:" << gitOut;


	// git push
	
	QStringList pushArgs;
	pushArgs << "push";

	gitproc->start(git, pushArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	gitOut = gitproc->readAll();

	qDebug() << "Git push output:" << gitOut;
}

