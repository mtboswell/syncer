#include "undelete.h"

Undelete::Undelete(QString path, QWidget* parent):QWidget(parent){
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

	treeWidget->setColumnCount(3);
	QStringList headers;
	headers << "Date" << "Author" << "Commit ID";
	treeWidget->setHeaderLabels(headers);

}

void Undelete::on_pushButton_clicked(){
	QTreeWidgetItem* selectedCommit = treeWidget->currentItem();
	if(!selectedCommit) return;
	QString commit = selectedCommit->text(2);

	//qDebug() << "Restoring" << selectedFilePath << "from commit" << commit;

	QStringList checkoutArgs;
	checkoutArgs << "checkout" << commit+"^" << "--" << selectedFilePath;

	gitproc->start(git, checkoutArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	QString gitOut = gitproc->readAll();

	//qDebug() << "Git checkout output:" << gitOut;

	QMessageBox::information (this, "Restore succeeded", "Successfully restored file " + selectedFilePath + " from commit " + commit);
	
}

void Undelete::fileSelected(QListWidgetItem* item){
	QString path = item->text();
	selectedFilePath = path;


	QStringList logArgs;
	logArgs << "log" << "--" << path;

	gitproc->start(git, logArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	QString gitOut = gitproc->readAll();

	//qDebug() << "Git log output:" << gitOut;

	QRegExp logReg("commit\\s+(\\w+)\\nAuthor:\\s+([\\w<@>\\.\\s]+)\\nDate:\\s+([\\w:-\\s]+) -[\\d]{4}\\n\\n([^\\n]+)\\n\\n?");

	int pos = 0;

	int row = 0;
	int column = 0;

	treeWidget->clear();

	row++;

	QList<QTreeWidgetItem *> items;

	while((pos = logReg.indexIn(gitOut, pos)) != -1){
		
		QString commit = logReg.cap(1);
		QString author = logReg.cap(2);
		QString date = logReg.cap(3).simplified();
		QString message = logReg.cap(4);

		// Mon Dec 19 22:24:49 2011 -0500
		QDateTime datetime = QDateTime::fromString(date, "ddd MMM dd HH:mm:ss yyyy");

		//qDebug() << "Processing commit" << commit;
		//qDebug() << "With Date:" << datetime;

		QStringList item;
		item << datetime.toString("yyyy-MM-dd hh:mm:ss ap") << author << commit;

		QTreeWidgetItem* treeItem = new QTreeWidgetItem((QTreeWidget*)0, item);

		items.append(treeItem);

		pos += logReg.matchedLength();
		row++;
	}

	treeWidget->insertTopLevelItems(0, items);
	treeWidget->sortItems(0, Qt::DescendingOrder);
}
