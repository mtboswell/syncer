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
		qFatal("Shell did not start!");
	}else{
		shProc->write("PS1=$\n");
		expect("PS1=\\$");
		expectEnd();
		run("PATH=$PATH:$PWD");
		expectEnd();
		//shProc->waitForReadyRead();
		//qDebug() << "Started:" << shProc->readAllStandardError() << shProc->readAllStandardOutput();
		//buf.clear();
	}
}

ShellRunner::~ShellRunner(){
	delete shProc;
}

QString ShellRunner::result(){
	return buf;
}

bool ShellRunner::expect(QString lookFor, int timeout){
	//qDebug() << "Expecting:" << lookFor;
	//qDebug() << "Have so far:" << buf;

	QTime timer;
	timer.start();

	while((!buf.contains(QRegExp(lookFor))) && (timer.elapsed() < timeout)){
		//qDebug() << "Waiting for read";
		if(!shProc->waitForReadyRead(timeout)){
			emit error("Command not responding", "Command " + currentCmd + " is not responding");
			qDebug() << "Read timed out" << "Command " + currentCmd + " is not responding";
			return false;
		}

		buf += shProc->readAll();

		if(buf.startsWith("$")) buf = buf.right(buf.size() - 1);
		if(buf.startsWith(currentCmd)) buf = buf.right(buf.size() - currentCmd.size());

		//qDebug() << "Expect Read:" << buf;
	}

	if(timer.elapsed() >= timeout){
		qDebug() << "Expect timed out";
		return false;
	}

	//qDebug() << "Expect found: " << lookFor;

	return true;
}

bool ShellRunner::have(QString lookFor){
	return buf.contains(lookFor);
}

void ShellRunner::run(QString cmd){
	stop();

	//qDebug() << "Running: " << cmd;

	currentCmd = cmd;

	shProc->write(QString(cmd + "\n").toLatin1());

	if(expect(cmd))
		buf.clear();
	else	
		qDebug() << "Command did not start:" << buf;
}

bool ShellRunner::run(QString cmd, QString exp){
	run(cmd);
	return expect(exp);
}

void ShellRunner::onReadyReadStdOut(){
	//qDebug() << "Reading StdOut";
	buf += shProc->readAllStandardOutput();
}
void ShellRunner::onReadyReadStdErr(){
	//qDebug() << "Reading StdErr";
	buf += shProc->readAllStandardError();
}


void ShellRunner::stop(){
	//qDebug() << "Stopping";
	char etx = QChar(3).toLatin1();
	//qDebug() << etx;
	while(!buf.contains("$")){
		shProc->write(&etx, 1);
		expectEnd();
	}
	buf.clear();
}

void ShellRunner::exit(){
	stop();
	shProc->write("exit\n");
	if(!shProc->waitForFinished()) qFatal("Shell did not exit!");
}

bool ShellRunner::expectEnd(int timeout){
	return expect("\\$$", timeout);
}

bool ShellRunner::cd(QString path){
	run("cd " + path);
	expectEnd();
	return run("pwd", path);
}

bool ShellRunner::runToEnd(QString cmd, int timeout){
	run(cmd);
	if(!expectEnd(timeout)) {
		qFatal(QString(cmd+" did not end!").toLatin1());
	}
	return true;
}


