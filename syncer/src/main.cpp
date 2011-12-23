
#include <QApplication>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QDir>
#include <QFileInfoList>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include "gitlauncher.h"

QStringList recursePaths(QString baseDir);

int main(int argc, char** argv){
	QApplication app(argc, argv);

	QStringList inputPaths, pathsToMonitor;

	QStringList args = app.arguments();
	args.removeFirst();
	foreach(QString arg, args){
		if(arg.startsWith("--")){
		
		}else{
			inputPaths << arg;
		}
	}


	foreach(QString path, inputPaths){
		pathsToMonitor << recursePaths(path);
	}

	qDebug() << "Monitoring:" << pathsToMonitor;

	QFileSystemWatcher watch;

	watch.addPaths(pathsToMonitor);

	GitLauncher* gitlauncher = new GitLauncher();

	QObject::connect(&watch, SIGNAL(fileChanged(QString)), gitlauncher, SLOT(fileChanged(QString)));
	QObject::connect(&watch, SIGNAL(directoryChanged(QString)), gitlauncher, SLOT(directoryChanged(QString)));

	QTimer pullTimer;

	QObject::connect(&pullTimer, SIGNAL(timeout()), gitlauncher, SLOT(checkForUpdate()));

	pullTimer.start(60000);

	foreach(QString path, pathsToMonitor){
		gitlauncher->directoryChanged(path);
	}

	app.exec();

}



QStringList recursePaths(QString baseDir){
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
