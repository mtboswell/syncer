#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QStringList>

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

};
