#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QStringList>
#include <QSettings>
#include <QTextStream>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QDir>
#include <QFileInfoList>
#include <QTimer>
#include <QSettings>
#include <QDebug>

#include "../shared/shellrunner.h"

class GitLauncher : public QObject {
	Q_OBJECT
	public:
		GitLauncher(QString pathToRepository = ".");

	private slots:
		void directoryChanged(QString path);
		void checkForUpdate();
		void doPush();
		
	private:
		QStringList recursePaths(QString baseDir);
		ShellRunner* sh;
		QFileSystemWatcher* watch;
		QTimer* pushTimer;
		QTimer* pullTimer;
		QStringList dirsChanged;
		int pushDelay;
		QString repoPath;

};
