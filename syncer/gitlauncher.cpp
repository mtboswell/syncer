#include "gitlauncher.h"
#include <QDebug>

GitLauncher::GitLauncher(){
	QSettings* settings = new QSettings("MiBoSoft", "Syncer");

	pushDelay = settings->value("pushDelay", 5000).toInt();

	sh = new ShellRunner;

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

	qDebug() << "Pulling";
	sh->runToEnd("git pull --rebase");

	if(!sh->have("Already up-to-date.") && !sh->have("up to date")){
		if(sh->have("fatal"))
			out << "Error: " << gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6) << endl;
		else if(sh->have("Updating"))
			out << "Synchronized from server" << endl;
		else
			out << "Unknown Error: " << sh->result() << endl;
		
	}
}

void GitLauncher::doPush(){
	pushTimer->stop();
	QTextStream out(stdout);

	// git add each dir
	foreach(QString path, dirsChanged){
		//qDebug() << "Adding" << path;
		if(!sh->cd(path)) qDebug() << "Could not cd to " << path << sh->result();

		// git add dir
		qDebug() << "Adding" << path;
		sh->runToEnd("git add --all");
		if(sh->have("fatal")){
			qDebug() << "Failed to add dir" << path;
		}

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
	sh->runToEnd("git push origin master", 15000);
	
	if(!sh->have("Already up-to-date.")){
		if(sh->have("fatal"))
			out << "Error: " << gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6) << endl;
		else if(sh->have("master -> master"))
			out << "Synchronized to server" << endl;
		else
			out << "Unknown Error: " << sh->result() << endl;
		
	}
	
}

