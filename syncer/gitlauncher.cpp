#include "gitlauncher.h"
#include <QDebug>

GitLauncher::GitLauncher(QString pathToRepository){
	// Get settings
	QSettings* settings = new QSettings("MiBoSoft", "Syncer");
	pushDelay = settings->value("pushDelay", 5000).toInt();

	repoPath = pathToRepository;

	QDir gitDir(repoPath+"/.git");
	if(!gitDir.exists()){
		qFatal(QString(repoPath+" is not a git repository!").toLatin1());
	}

	// initalize shell
	sh = new ShellRunner;
	if(!sh->cd(repoPath)) qDebug() << "Error: could not cd to repo";

	// setup timers
	pushTimer = new QTimer();
	connect(pushTimer, SIGNAL(timeout()), this, SLOT(doPush()));

	pullTimer = new QTimer();
	QObject::connect(pullTimer, SIGNAL(timeout()), this, SLOT(checkForUpdate()));

	// setup filesystemwatcher
	watch = new QFileSystemWatcher();
	QStringList pathsToMonitor = recursePaths(repoPath);
	watch->addPaths(pathsToMonitor);
	QObject::connect(watch, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));

	// start pulling
	//QTimer::singleShot(100, this, SLOT(checkForUpdate()));
	pullTimer->start(settings->value("pullDelay", 60000).toInt());

	// push
	foreach(QString path, pathsToMonitor){
		directoryChanged(path);
	}

}

void GitLauncher::directoryChanged(QString path){
	//qDebug() << "Directory Changed:" << path;
	if(!dirsChanged.contains(path))
		dirsChanged << path;
	pushTimer->start(pushDelay);

}

void GitLauncher::checkForUpdate(){

	QTextStream out(stdout);

	// git pull

	qDebug() << "Pulling";
	sh->runToEnd("git pull --rebase");

	if(!sh->have("Already up-to-date.") && !sh->have("up to date")){
		if(sh->have("fatal"))
			out << "Error: " << sh->result().right(sh->result().size() - sh->result().indexOf("fatal:") - 6) << endl;
		else if(sh->have("Updating") || sh->have("Fast-forwarded"))
			out << "Synchronized from server" << endl;
		else
			out << "Unknown Error: " << sh->result() << endl;
		
	}
}

void GitLauncher::doPush(){
	pushTimer->stop();
	QTextStream out(stdout);

	// git add each dir
	/*
	foreach(QString path, dirsChanged){
		// git add dir
		qDebug() << "Adding" << path;
		sh->runToEnd("git add --all " + path);
		if(sh->have("fatal")){
			qDebug() << "Failed to add dir" << path;
		}
	}
	*/

	sh->runToEnd("git add --all");
	if(sh->have("fatal")){
		qDebug() << "Failed to add";
	}

	dirsChanged.clear();

	// git commit
	// note that the commit happens in the last dir that git add was run in. This means we don't support multiple repos per process.

	qDebug() << "Committing";
	sh->runToEnd("git commit -m 'Autosync commit'");
	if(!sh->have("nothing to commit")) out << "Synchronized with local" << endl;
	else return;

	// git push

	qDebug() << "Pushing";
	sh->runToEnd("git push origin master", 35000);
	
	if(!sh->have("Already up-to-date.")){
		if(sh->have("fatal"))
			out << "Error: " << sh->result().right(sh->result().size() - sh->result().indexOf("fatal:") - 6) << endl;
		else if(sh->have("master -> master"))
			out << "Synchronized to server" << endl;
		else
			out << "Unknown Error: " << sh->result() << endl;
		
	}
	
}

QStringList GitLauncher::recursePaths(QString baseDir){
	QStringList subPaths;

	QFileInfo info(baseDir);
	if(info.isDir()){
		subPaths << baseDir;
		QDir dir(baseDir);
		dir.setFilter(QDir::Dirs | QDir::NoDotDot | QDir::NoDot);

		QFileInfoList dirlist = dir.entryInfoList();

		for(int i = 0; i < dirlist.size(); ++i){
			QFileInfo dirInfo = dirlist.at(i);
			//qDebug() << "Found subdir:" << dirInfo.fileName();
			subPaths << recursePaths(dir.cleanPath(baseDir) + "/" + dirInfo.fileName());
		}

	}else qDebug() << "Error:" << baseDir << "is not a directory";
	return subPaths;

}

