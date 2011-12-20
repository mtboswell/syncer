#include "restore.h"

Restore::Restore(QString path, QWidget* parent):QWidget(parent){
	setupUi(this);

	files = new QFileSystemModel;
	files->setRootPath(path);

	treeView->setModel(files);
	treeView->setRootIndex(files->index(path));

	connect(treeView, SIGNAL(activated(QModelIndex)), this, SLOT(fileSelected(QModelIndex)));
	
	QTableWidgetItem *testItem = new QTableWidgetItem("test");
	tableWidget->setItem(1, 1, testItem);

}

void Restore::on_pushButton_clicked(){
	
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

	QRegExp logReg("commit\\s+(\\w+)\\nAuthor:\\s+([\\w<@>\\s]+)\\nDate:\\s+([\\w:-\\s]+)\\n([\\w\\\"\\s]+)\n");

	int pos = 0;

	int row = 0;
	int column = 0;

	tableWidget->clear();

	row++;


	QTableWidgetItem *testItem = new QTableWidgetItem("test");
	tableWidget->setItem(row, 0, testItem);
	row++;

	while((pos = logReg.indexIn(gitOut, pos)) != -1){
		
		QString commit = logReg.cap(1);
		QString author = logReg.cap(2);
		QString date = logReg.cap(3);
		QString message = logReg.cap(4);

		QTableWidgetItem *authorItem = new QTableWidgetItem(author);
		tableWidget->setItem(row, 0, authorItem);

		QTableWidgetItem *dateItem = new QTableWidgetItem(date);
		tableWidget->setItem(row, 1, dateItem);

		QTableWidgetItem *commitItem = new QTableWidgetItem(commit);
		tableWidget->setItem(row, 2, commitItem);

		pos += logReg.matchedLength();
		row++;
	}


}
