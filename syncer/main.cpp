
#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include "gitlauncher.h"

int main(int argc, char** argv){
	QCoreApplication app(argc, argv);
	QTextStream out(stdout);

	QStringList inputPaths, pathsToMonitor;

	QStringList args = app.arguments();
	args.removeFirst();

	if(args.size() == 0) return 1;

	foreach(QString arg, args){
		if(arg.startsWith("--")){
			// options go here
		}else{
			inputPaths << arg;
		}
	}

	QList<GitLauncher*> gitlaunchers;

	int i = 0;
	foreach(QString path, inputPaths){
		gitlaunchers.append(new GitLauncher(path));
		i++;
	}

	return app.exec();
}



