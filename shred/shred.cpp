#include "shred.h"

Shred::Shred(QString path, QWidget* parent):QWidget(parent){
	setupUi(this);
	
	gitproc = new QProcess(this);
	gitproc->setProcessChannelMode(QProcess::MergedChannels);
	git = "git";

	gitproc->setWorkingDirectory(path);
	QStringList logArgs;
	//git log --all --pretty=format: --name-only --diff-filter=D
	logArgs << "log" << "--all" << "--pretty=format:" << "--name-only" << "--diff-filter=D";

	gitproc->start(git, logArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	QString gitOut = gitproc->readAll();

	//qDebug() << "Git log output:" << gitOut;

	QStringList files = gitOut.split('\n');


	foreach(QString file, files){
		if(file != "")
			new QListWidgetItem(file, listWidget);
	}

	connect(listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(fileSelected(QListWidgetItem*)));


}

void Shred::on_shredButton_clicked(){
	// figure out which file we have

	selectedFilePath = listWidget->currentItem()->text();


	// check to make sure we're sure

	if(QMessageBox::warning(this, "About to Shred!", "You are trying to shred the file " + selectedFilePath + ". This file cannot be recovered. Is this ok?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok)
		return;

	QString ret;

	// git pull
	ret = runGit(QStringList() << "pull");
	qDebug() << ret;
	if(ret.isNull()) return;

	// git add .
	ret = runGit(QStringList() << "add" << ".");
	qDebug() << ret;
	if(ret.isNull()) return;

	// git commit
	ret = runGit(QStringList() << "commit" << "-m Shred Commit");
	qDebug() << ret;
	if(ret.isNull()) return;

	// git filter-branch --index-filter 'git rm --cached --ignore-unmatch filename'
	ret = runGit(QStringList() << "filter-branch" << "--index-filter" << "git rm --cached --ignore-unmatch " + selectedFilePath);
	qDebug() << ret;
	if(ret.contains("Cannot rewrite branches")){
		QMessageBox::warning(this, "Git Error", ret);
		return;
	}
	if(ret.isNull()) return;

	// rm -Rf .git/refs/original
	ret = runProc("rm", QStringList() << "-rf" << ".git/refs/original");
	qDebug() << ret;
	if(ret.isNull()) return;

	// rm -Rf .git/logs/
	ret = runProc("rm", QStringList() << "-rf" << ".git/logs");
	qDebug() << ret;
	if(ret.isNull()) return;

	// git gc --prune=now
	ret = runGit(QStringList() << "gc" << "--prune=now");
	qDebug() << ret;
	if(ret.isNull()) return;

	// git push -f
	ret = runGit(QStringList() << "push" << "-f");
	qDebug() << ret;
	if(ret.isNull()) return;


	// done!
	QMessageBox::information(this, "Success", "File " + selectedFilePath + " sucessfully shredded.");

	
}

void Shred::fileSelected(QListWidgetItem* item){
	QString path = item->text();
	selectedFilePath = path;
}

QString Shred::runGit(QStringList args){

	QString gitOut = runProc(git, args);

	if(gitOut.contains("fatal")){
		qDebug() << "Error:" << gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6) << "\n";
		QMessageBox::critical (this, "Error", gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6));
		return QString();
	}

	return gitOut;

}
QString Shred::runProc(QString app, QStringList args){

	gitproc->start(app, args);

	if(!gitproc->waitForStarted()){
		qDebug() << "Error: "+app+" did not start (" << gitproc->error() << ")";
		QMessageBox::critical (this, "Error", app+" did not start");
		return QString();
	}
	if(!gitproc->waitForFinished()){
		qDebug() << "Error: "+app+" did not finish (" << gitproc->error() << ")";
		QMessageBox::critical (this, "Error", app+" did not finish");
		return QString();
	}
	QString gitOut = gitproc->readAll();

	return gitOut;

}
