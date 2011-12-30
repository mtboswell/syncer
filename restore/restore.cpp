#include "restore.h"

Restore::Restore(QString path, QWidget* parent):QWidget(parent){
	setupUi(this);
	retranslateUi(this);

#ifdef Q_WS_WIN
	git = "git";
#else
    git = "git";
#endif

	files = new QFileSystemModel;
	files->setRootPath(path);
	fileRootPath->setText(files->rootPath());

	connect(files, SIGNAL(directoryLoaded(QString)), this, SLOT(onDirLoaded(QString)));

	treeView->setModel(files);
	treeView->setRootIndex(files->index(path));

	connect(treeView, SIGNAL(activated(QModelIndex)), this, SLOT(fileSelected(QModelIndex)));
//	connect(treeView, SIGNAL(expanded(QModelIndex)), this, SLOT(selectExpanded(QModelIndex)));


	treeWidget->setColumnCount(3);
	QStringList headers;
	headers << "Date" << "Author" << "Commit ID";
	treeWidget->setHeaderLabels(headers);


}

void Restore::on_restoreButton_clicked(){

	QTreeWidgetItem* selectedCommit = treeWidget->currentItem();
	if(!selectedCommit) return;
	QString commit = selectedCommit->text(2);

	QModelIndex selectedFileIndex = treeView->currentIndex();
	QFileInfo selectedFileInfo = files->fileInfo(selectedFileIndex);
	QString selectedFilePath = files->filePath(selectedFileIndex);

	//qDebug() << "Restoring" << selectedFilePath << "from commit" << commit;

	QProcess *gitproc = new QProcess();
	QStringList checkoutArgs;
	checkoutArgs << "checkout" << commit << selectedFilePath;

	gitproc->start(git, checkoutArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	QString gitOut = gitproc->readAll();

	//qDebug() << "Git checkout output:" << gitOut;

	 QMessageBox::information (this, "Restore succeeded", "Successfully restored file " + selectedFilePath + " from commit " + commit);
	
}

void Restore::fileSelected(QModelIndex index){
	QString path = files->filePath(index);

	QFileInfo pathInfo = files->fileInfo(index);

	QProcess *gitproc = new QProcess();

	gitproc->setWorkingDirectory(pathInfo.dir().absolutePath());

	QStringList logArgs;
	logArgs << "log" << path;

	gitproc->start(git, logArgs);
	if(!gitproc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return;
	}
	if(!gitproc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		return;
	}
	QString gitOut = gitproc->readAll();

	//qDebug() << "Git log output:" << gitOut;
	//QMessageBox::information (this, "Git Log for " + path, gitOut);

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
		//qDebug() << "With Date:" << date << datetime;

		QStringList item;
		item << datetime.toString("yyyy-MM-dd hh:mm:ss ap") << author << commit;

		QTreeWidgetItem* treeItem = new QTreeWidgetItem((QTreeWidget*)0, item);

		items.append(treeItem);

		pos += logReg.matchedLength();
		row++;
	}

	treeWidget->insertTopLevelItems(0, items);
	treeWidget->sortItems(0, Qt::DescendingOrder);

		//QMessageBox::information (this, "Done", "Finished adding revisions.");

}


void Restore::on_upButton_clicked(){
	files->setRootPath(files->rootPath() + "/..");
	fileRootPath->setText(files->rootPath());
	treeView->setRootIndex(files->index(files->rootPath()));
}

void Restore::on_goButton_clicked(){
	files->setRootPath(fileRootPath->text());
	fileRootPath->setText(files->rootPath());
	treeView->setRootIndex(files->index(files->rootPath()));
}

void Restore::selectExpanded(QModelIndex index){
	//treeView->resizeColumnToContents(0);
}

void Restore::onDirLoaded(QString path){
	treeView->expandAll();
	treeView->resizeColumnToContents(0);
	treeView->resizeColumnToContents(1);
	treeView->resizeColumnToContents(2);
	treeView->resizeColumnToContents(3);
	treeView->collapseAll();
}
