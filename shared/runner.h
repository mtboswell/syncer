#ifndef RUNNER_H
#define RUNNER_H

#include <QProcess>
#include <QString>
#include <QTime>
#include <QDebug>

struct RunResult {
	QProcess::ExitStatus status;
	QProcess::ProcessError error;
	int exitCode;
	QString stdOut;
	QString stdErr;
};

class Runner : public QObject
{
	Q_OBJECT
	public:

		static RunResult run(QString cmd, int timeout = 5000);
		static void setWorkingDirectory(QString dir);
	
	private:
		static QString workingDirectory;

};
#endif
