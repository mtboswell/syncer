#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QStringList>
#include <QSettings>
#include <QTextStream>

#include "../shared/shellrunner.h"

class GitLauncher : public QObject {
	Q_OBJECT
	public:
		GitLauncher();
	public slots:
		void fileChanged(QString path);
		void directoryChanged(QString path);
		void checkForUpdate();

	private slots:
		void doPush();
		
	private:
		ShellRunner* sh;
		QByteArray gitOut;
		QTimer* pushTimer;
		QStringList dirsChanged;
		int pushDelay;

};
