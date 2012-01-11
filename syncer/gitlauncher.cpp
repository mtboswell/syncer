#include "gitlauncher.h"
#include <QDebug>

GitLauncher::GitLauncher(){
#ifdef Q_WS_WIN
		git = "git";
#else
        git = "git";
#endif


	QSettings* settings = new QSettings("MiBoSoft", "Syncer");

	pushDelay = settings->value("pushDelay", 5000).toInt();

        gitproc = new QProcess();
	gitproc->setProcessChannelMode(QProcess::MergedChannels);
	pushTimer = new QTimer();
	connect(pushTimer, SIGNAL(timeout()), this, SLOT(doPush()));
}

void GitLauncher::fileChanged(QString path){
	//qDebug() << "File Changed:" << path;

}
void GitLauncher::directoryChanged(QString path){
	//qDebug() << "Directory Changed:" << path;
	if(!dirsChanged.contains(path))
		dirsChanged << path;
	pushTimer->start(pushDelay);

}

void GitLauncher::checkForUpdate(){

	QTextStream out(stdout);

	// cd to watched directory



	// git pull
	
	QStringList pullArgs;
	pullArgs << "pull" << "--rebase";

	gitproc->start(git, pullArgs);
	if(!gitproc->waitForStarted()){
		out << "Error: git did not start (" << gitproc->error() << ")";
		return;
	}
	if(!gitproc->waitForFinished()){
		out << "Error: git did not finish (" << gitproc->error() << ")";
		return;
	}
	gitOut = gitproc->readAll();

	//qDebug() << "Git pull output:" << gitOut;

	if(!gitOut.isEmpty() && !gitOut.contains("Already up-to-date.") && !gitOut.contains("up to date")){
		if(gitOut.contains("fatal"))
			out << "Error: " << gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6) << "\n";
		else if(gitOut.contains("Updating"))
			out << "Synchronized from server\n";
		else
			out << "Unknown Error: " << gitOut << "\n";
		
	}
}

void GitLauncher::doPush(){
	pushTimer->stop();
	QTextStream out(stdout);
	// git add each dir
	foreach(QString path, dirsChanged){
		//qDebug() << "Adding" << path;
		gitproc->setWorkingDirectory(path);

		// git add dir

		QStringList addArgs;
		addArgs << "add" << "--all";

		gitproc->start(git, addArgs);
		if(!gitproc->waitForStarted()){
			out << "Error: git did not start (" << gitproc->error() << ")\n";
			return;
		}
		if(!gitproc->waitForFinished()){
			out << "Error: git did not finish (" << gitproc->error() << ")\n";
			return;
		}
		gitOut = gitproc->readAll();

		//qDebug() << "Git add output:" << gitOut;
	}
	dirsChanged.clear();



	// git commit
	// note that the commit happens in the last dir that git add was run in. This means we don't support multiple repos per process.

	QStringList commitArgs;
	commitArgs << "commit" << "-m" << "Autosync commit";
	//qDebug() << "Commit args:" << commitArgs;

	gitproc->start(git, commitArgs);
	if(!gitproc->waitForStarted()){
		out << "Error: git did not start (" << gitproc->error() << ")\n";
		return;
	}
	if(!gitproc->waitForFinished()){
		out << "Error: git did not finish (" << gitproc->error() << ")\n";
		return;
	}
	gitOut = gitproc->readAll();


	//qDebug() << "Git commit output:" << gitOut;
	if(!gitOut.isEmpty() && !gitOut.contains("nothing to commit")) out << "Synchronized with local, ";
	else return;


	// git push
	
	QStringList pushArgs;
	pushArgs << "push" << "origin" << "master";

	gitproc->start(git, pushArgs);
	if(!gitproc->waitForStarted()){
		out << "Error: git did not start: " << gitproc->error() << "\n";
		return;
	}
	if(!gitproc->waitForFinished()){
		out << "Error: git did not finish: " << gitproc->error() << "\n";
		return;
	}
	gitOut = gitproc->readAll();

	//qDebug() << "Git push output:" << gitOut;
	if(!gitOut.isEmpty() && !gitOut.contains("Already up-to-date.")){
		if(gitOut.contains("fatal"))
			out << "Error: " << gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6) << "\n";
		else if(gitOut.contains("master -> master"))
			out << "Synchronized to server\n";
		else
			out << "Unknown Error: " << gitOut << "\n";
		
	}
	
}

