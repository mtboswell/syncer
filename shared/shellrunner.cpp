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
		if(!expect("$")){
			qDebug() << "Error: did not find initial prompt.";
			return ;
		}
		
		shProc->write("PS1=$+$\n");
		if(expect("PS1=$+$\n")){
			buf = buf.right(buf.size() - (buf.indexOf("PS1=$+$\n") + 8));
			state = Stopped;
		}else{
			qDebug() << "Error: did not set prompt";
			return ;
		}

		runToEnd("PATH=$PATH:$PWD");
		//qDebug() << "Starting buffer:" << buf;
		//shProc->waitForReadyRead();
		//qDebug() << "Started:" << shProc->readAllStandardError() << shProc->readAllStandardOutput();
		//buf.clear();
	}
}

ShellRunner::~ShellRunner(){
	delete shProc;
}

QString ShellRunner::result(){
	return buf.replace(QRegExp("\\n[^\\n]*\\$\\+\\$$"), "");
}

bool ShellRunner::expect(QString lookFor, int timeout){
	//qDebug() << "Expecting:" << lookFor;
	//qDebug() << "Have so far:" << buf;

	QTime timer;
	timer.start();

	while((!buf.contains(lookFor)) && (timer.elapsed() < timeout)){
		//qDebug() << "Waiting for read";
		if(!shProc->waitForReadyRead(timeout)){
			qDebug() << "Read timed out" << "Did not find " + lookFor + " in output";
			return false;
		}

		buf += shProc->readAll();

		//qDebug() << "Expect Read:" << buf;
	}

	if(timer.elapsed() >= timeout){
		qDebug() << "Expect timed out";
		return false;
	}

	//qDebug() << "Expect found: " << lookFor;

	return true;
}
bool ShellRunner::expectRegExp(QString lookFor, int timeout){
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

	qDebug() << "Running: " << cmd;

	currentCmd = cmd;

	shProc->write(QString(cmd + "\n").toLatin1());

	state = Running;

	if(expect(cmd + "\n")){
		//qDebug() << "Found cmd, cutting from:" << buf;
		buf = buf.right(buf.size() - (buf.indexOf(cmd) + cmd.size() + 1));
		//qDebug() << "After cut:" << buf;
	}else{	
		qDebug() << "Command did not start:" << buf;
	}
}

bool ShellRunner::run(QString cmd, QString exp){
	run(cmd);
	return expect(exp);
}

void ShellRunner::onReadyReadStdOut(){
	buf += shProc->readAllStandardOutput();
	//qDebug() << "Reading StdOut" << buf;
}
void ShellRunner::onReadyReadStdErr(){
	buf += shProc->readAllStandardError();
	//qDebug() << "Reading StdErr" << buf;
}


void ShellRunner::stop(){
	if(state == Stopped) return;
	//qDebug() << "Stopping";
	currentCmd = "STOP";

	char etx = QChar(3).toAscii();
	//qDebug() << etx;
	while(!buf.contains("$+$")){
		shProc->write(&etx, 1);
		expectEnd();
	}
	buf.clear();
	state = Stopped;
}

void ShellRunner::exit(){
	stop();
	shProc->write("exit\n");
	if(!shProc->waitForFinished()) qFatal("Shell did not exit!");
}

bool ShellRunner::expectEnd(int timeout){
	if(expectRegExp("\\$\\+\\$$", timeout)){
		state = Stopped;
		return true;
	}else{
		return false;
	}
}

bool ShellRunner::cd(QString path){
	path.replace("\\","/");
	// remove colon after drive letter
	path.replace(QRegExp("([A-Z]):"), "/\\1");

	while(path.endsWith("/")) path = path.left(path.size() - 1);

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


