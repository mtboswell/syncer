#include "shellrunner.h"
#include <QTime>
#include <QDebug>

ShellRunner::ShellRunner(QString workingdir, QString shell){
	shProc = new QProcess(this);
	shProc->setWorkingDirectory(workingdir);
	shProc->setProcessChannelMode(QProcess::MergedChannels);

	connect(shProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(finished()));
	connect(shProc, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStdOut()));
	connect(shProc, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStdErr()));
	
	shProc->start(shell);
	if(!shProc->waitForStarted()){
		qDebug() << shell << " did not start!";
	}else{
		shProc->write("PS1=$\n");
		expect("PS1=\\$");
		buf.clear();
		//shProc->waitForReadyRead();
		qDebug() << "Started:" << shProc->readAllStandardError() << shProc->readAllStandardOutput();
		//buf.clear();
	}
}

ShellRunner::~ShellRunner(){
	delete shProc;
}

QString ShellRunner::expect(QString lookFor){
	qDebug() << "Expecting:" << lookFor;
	qDebug() << "Have so far:" << buf;

	QTime timer;
	timer.start();

	while((!buf.simplified().contains(QRegExp(lookFor))) && (timer.elapsed() < 5000)){
		qDebug() << "Waiting for read";
		if(!shProc->waitForReadyRead(2000)){
			emit error("Command not responding", "Command " + currentCmd + " is not responding");
			qDebug() << "Read timed out";
			return QString();
		}

		buf += shProc->readAll();

		if(buf.startsWith("$")) buf = buf.right(buf.size() - 1);
		if(buf.startsWith(currentCmd)) buf = buf.right(buf.size() - currentCmd.size());

		qDebug() << "Expect Read:" << buf;
	}

	if(timer.elapsed() >= 5000){
		qDebug() << "Expect timed out";
		return QString();
	}

	qDebug() << "Expect found: " << lookFor;

	return buf;
}

bool ShellRunner::have(QString lookFor){
	return buf.contains(lookFor);
}

void ShellRunner::run(QString cmd){
	stop();

	qDebug() << "Running: " << cmd;

	currentCmd = cmd;

	shProc->write(QString(cmd + "\n").toLatin1());
}

QString ShellRunner::run(QString cmd, QString exp){
	run(cmd);
	return expect(exp);
}

void ShellRunner::onReadyReadStdOut(){
	qDebug() << "Reading StdOut";
	buf += shProc->readAllStandardOutput();
}
void ShellRunner::onReadyReadStdErr(){
	qDebug() << "Reading StdErr";
	buf += shProc->readAllStandardError();
}


void ShellRunner::stop(){
	qDebug() << "Stopping";
	char etx = QChar(3).toLatin1();
	qDebug() << etx;
	while(!buf.contains("$")){
		shProc->write(&etx, 1);
		expect("\\$");
	}
	buf.clear();
}

void ShellRunner::exit(){
	stop();
	shProc->write("exit\n");
	shProc->waitForFinished();
}

