#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QStringList>
<<<<<<< HEAD
=======
#include <QSettings>
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581
#include <QTextStream>

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
		QString git;
		QProcess* gitproc;
		QByteArray gitOut;
		QTimer* pushTimer;
		QStringList dirsChanged;
<<<<<<< HEAD
=======
		int pushDelay;
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581

};
