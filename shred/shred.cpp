#include "shred.h"

Shred::Shred(QString path, QWidget* parent):QWidget(parent){
	setupUi(this);
	
	gitproc = new QProcess(this);
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

	// check to make sure we're sure

	// git pull

	// git filter-branch --index-filter 'git rm --cached --ignore-unmatch filename'

	// rm -Rf .git/refs/original

	// rm -Rf .git/logs/

	// git gc

	// git prune --expire

	// git push -f


/*

	QStringList checkoutArgs;
	checkoutArgs << "checkout" << commit+"^" << "--" << selectedFilePath;

	gitproc->start(git, checkoutArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	QString gitOut = gitproc->readAll();

	//qDebug() << "Git checkout output:" << gitOut;

	QMessageBox::information (this, "Restore succeeded", "Successfully restored file " + selectedFilePath + " from commit " + commit);

	*/
	
}

void Shred::fileSelected(QListWidgetItem* item){
	QString path = item->text();
	selectedFilePath = path;
}
