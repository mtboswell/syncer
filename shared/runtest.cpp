#include "runner.h"
#include <QDebug>


int main(int argc, char** argv){
	
	QStringList cmds;

	cmds << "ls -l" << "l;kasjdf" << "ssh localhost";

	Runner::setWorkingDirectory("..");

	foreach(QString cmd, cmds){
		qDebug() << "Running:" << cmd;

		RunResult res = Runner::run(cmd);

		qDebug() << "Exit Status:" << res.status;
		qDebug() << "Error:" << res.error;
		qDebug() << "StdOut:" << res.stdOut;
		qDebug() << "StdErr:" << res.stdErr;

	}

	return 0;
	
}
