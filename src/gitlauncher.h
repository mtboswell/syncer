#include <QProcess>
#include <QObject>

class GitLauncher : public QObject {
	Q_OBJECT
	public:
		GitLauncher();
	public slots:
		void fileChanged(QString path);
		void directoryChanged(QString path);
		void checkForUpdate();
		

};
