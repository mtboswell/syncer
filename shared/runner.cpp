#include "runner.h"

QString Runner::workingDirectory = ".";

RunResult Runner::run(QString cmd, int timeout){
	QProcess proc;
	QTime procTimer;
	RunResult out;

	proc.setWorkingDirectory(workingDirectory);

	proc.start(cmd);

	if(!proc.waitForStarted()){
		qDebug() << "Error: process" << cmd << "did not start.";
		out.error = proc.error();
		out.status = proc.exitStatus();
		return out;
	}

	procTimer.start();

	while(((procTimer.elapsed() < timeout) || (timeout == -1)) && proc.state() == QProcess::Running){
		proc.waitForFinished(timeout/10);
		//if(proc.waitForReadyRead(timeout/10)){
			out.stdOut += proc.readAllStandardOutput();
			out.stdErr += proc.readAllStandardError();
		//}
	}

	if(proc.state() != QProcess::NotRunning){
		qDebug() << "Error: process" << cmd << "did not finish in" << timeout << "msecs.";
		proc.terminate();
		if(proc.state() != QProcess::NotRunning)
			proc.kill();
	}

	out.error = proc.error();
	out.status = proc.exitStatus();
	return out;
}

void Runner::setWorkingDirectory(QString dir){
	workingDirectory = dir;
}
