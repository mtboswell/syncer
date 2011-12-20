#include "restore.h"

Restore::Restore(QString path, QWidget* parent):QWidget(parent){
	setupUi(this);

	files = new QFileSystemModel;
	files->setRootPath(path);

	treeView->setModel(files);
	treeView->setRootIndex(files->index(path));

	connect(treeView, SIGNAL(activated(QModelIndex)), this, SLOT(fileSelected(QModelIndex)));
	

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
	QByteArray gitOut = gitproc->readAll();

	qDebug() << "Git log output:" << gitOut;

	QRegExp logReg("commit (\\w+)\\nAuthor:\\s([\\w<>]+)\\nDate:\\s([\\w:-]+)\\n([\w\\\"]+)\n");


	int row = 0;
	int column = 0;


	QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row+1)*(column+1)));
	tableWidget->setItem(row, column, newItem);


}
