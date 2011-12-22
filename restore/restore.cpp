#include "restore.h"

Restore::Restore(QString path, QWidget* parent):QWidget(parent){
	setupUi(this);

	files = new QFileSystemModel;
	files->setRootPath(path);

	treeView->setModel(files);
	treeView->setRootIndex(files->index(path));

	connect(treeView, SIGNAL(activated(QModelIndex)), this, SLOT(fileSelected(QModelIndex)));

	treeWidget->setColumnCount(3);
	QStringList headers;
	headers << "Date" << "Author" << "Commit ID";
	treeWidget->setHeaderLabels(headers);

}

void Restore::on_pushButton_clicked(){

	QTreeWidgetItem* selectedCommit = treeWidget->currentItem();
	QString commit = selectedCommit->text(2);

	QModelIndex selectedFileIndex = treeView->currentIndex();
	QFileInfo selectedFileInfo = files->fileInfo(selectedFileIndex);
	QString selectedFilePath = files->filePath(selectedFileIndex);
	

	qDebug() << "Restoring" << selectedFilePath << "from commit" << commit;
	
}

void Restore::fileSelected(QModelIndex index){
	QString path = files->filePath(index);


	QProcess *gitproc = new QProcess();
	QString git = "git";
	QStringList logArgs;
	logArgs << "log" << path;

	gitproc->start(git, logArgs);
	if(!gitproc->waitForStarted()) return;
	if(!gitproc->waitForFinished()) return;
	QString gitOut = gitproc->readAll();

	qDebug() << "Git log output:" << gitOut;

	QRegExp logReg("commit\\s+(\\w+)\\nAuthor:\\s+([\\w<@>\\.\\s]+)\\nDate:\\s+([\\w:-\\s]+) -[\\d]{4}\\n\\n([\\w\"\\s]+)\\n?");

	int pos = 0;

	int row = 0;
	int column = 0;

	treeWidget->clear();

	row++;

	QList<QTreeWidgetItem *> items;

	while((pos = logReg.indexIn(gitOut, pos)) != -1){
		
		QString commit = logReg.cap(1);
		QString author = logReg.cap(2);
		QString date = logReg.cap(3);
		QString message = logReg.cap(4);

		// Mon Dec 19 22:24:49 2011 -0500
		QDateTime datetime = QDateTime::fromString(date, "ddd MMM dd HH:mm:ss yyyy");

		qDebug() << "Processing commit" << commit;
		qDebug() << "With Date:" << datetime;

		QStringList item;
		item << date << author << commit;

		QTreeWidgetItem* treeItem = new QTreeWidgetItem((QTreeWidget*)0, item);

		items.append(treeItem);

		pos += logReg.matchedLength();
		row++;
	}

	treeWidget->insertTopLevelItems(0, items);
	treeWidget->sortItems(0, Qt::DescendingOrder);

}
