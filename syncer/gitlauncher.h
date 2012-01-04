#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QStringList>
#include <QSettings>
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
		int pushDelay;

};
