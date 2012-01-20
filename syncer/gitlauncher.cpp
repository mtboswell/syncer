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

	Runner::setWorkingDirectory(repoPath);

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
	RunResult pullRes = Runner::run("git pull --rebase", 135000);

	if(!pullRes.stdOut.contains("Already up-to-date.") && !pullRes.stdOut.contains("up to date")){
		if(pullRes.stdErr.contains("fatal"))
			out << "Error: " << pullRes.stdErr.right(pullRes.stdOut.size() - pullRes.stdErr.indexOf("fatal:") - 6) << endl;
		else if(pullRes.stdOut.contains("Updating") || pullRes.stdOut.contains("Fast-forwarded"))
			out << "Synchronized from server" << endl;
		else
			out << "Unknown Error: " << pullRes.stdOut << pullRes.stdErr << endl;
	}
}

void GitLauncher::doPush(){
	pushTimer->stop();
	QTextStream out(stdout);

	RunResult addRes = Runner::run("git add --all");
	if(addRes.stdErr.contains("fatal")){
		qDebug() << "Error: Failed to add";
		return;
	}

	dirsChanged.clear();

	// git commit
	// note that the commit happens in the last dir that git add was run in. This means we don't support multiple repos per process.

	qDebug() << "Committing";
	RunResult commitRes = Runner::run("git commit -m 'Autosync commit'", 10000);
	if(commitRes.status){
		qDebug() << "Commit failed with error:" << commitRes.error;
		return;
	}
	qDebug() << "Commit output:" << commitRes.stdOut << commitRes.stdErr;
	if(!commitRes.stdOut.contains("nothing to commit")) out << "Synchronized with local" << endl;
	else return;

	// git push

	qDebug() << "Pushing";
	RunResult pushRes = Runner::run("git push origin master", 235000);
	if(pushRes.status){
		qDebug() << "Push failed with error:" << pushRes.error;
		return;
	}
	
	if(!pushRes.stdOut.contains("Already up-to-date.")){
		if(pushRes.stdErr.contains("fatal"))
			out << "Error: " << pushRes.stdErr.right(pushRes.stdErr.size() - pushRes.stdErr.indexOf("fatal:") - 6) << endl;
		else if(pushRes.stdOut.contains("master -> master"))
			out << "Synchronized to server" << endl;
		else
			out << "Unknown Error: " << pushRes.stdOut << pushRes.stdErr << endl;
		
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

