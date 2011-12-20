
#include <QApplication>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QObject>
#include <QTimer>
#include "gitlauncher.h"

int main(int argc, char** argv){
	QApplication app(argc, argv);

	QStringList pathsToMonitor;

	QStringList args = app.arguments();
	args.removeFirst();
	foreach(QString arg, args){
		if(arg.startsWith("--")){
		
		}else{
			pathsToMonitor << arg;
		}
	}

	QStringList subPaths;

	foreach(QString path, pathsToMonitor){
		QFileInfo info(path);
		if(info.isDir()){
			subPaths << path;
			QDir dir(path);
			dir.setFilter(QDir::Dirs | QDir::NoDotDot);

			QFileInfoList dirlist = dir.entryInfoList();

			for(int i = 0; i < dirlist.size(); ++i){
				QFileInfo dirInfo = dirlist.at(i);
				this(dirInfo.fileName());
			}

		}
	}



	QFileSystemWatcher watch;

	watch.addPaths(pathsToMonitor);

	GitLauncher* gitlauncher = new GitLauncher();

	QObject::connect(&watch, SIGNAL(fileChanged(QString)), gitlauncher, SLOT(fileChanged(QString)));
	QObject::connect(&watch, SIGNAL(directoryChanged(QString)), gitlauncher, SLOT(directoryChanged(QString)));

	QTimer pullTimer;

	QObject::connect(&pullTimer, SIGNAL(timeout()), gitlauncher, SLOT(checkForUpdate()));

	pullTimer.start(5000);

	app.exec();

}
