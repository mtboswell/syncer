#include <libssh/libssh.h>
#include <QObject>
#include <QString>

class RemoteShellRunner : public QObject {
	Q_OBJECT
	public:
		RemoteShellRunner(QString host, QString username, QString password, int port = 22);
		~RemoteShellRunner();
		bool isConnected();
		bool expect(QString lookFor, int timeout = 5000);
		bool expectRegExp(QString lookFor, int timeout = 5000);
		bool run(QString cmd, QString lookFor);
		bool have(QString lookFor);
		QString result();
		bool expectEnd(int timeout = 5000);
		bool cd(QString path);
		bool runToEnd(QString cmd, int timeout = 5000);

		enum CmdState {Stopped, Running} state;

	public slots:
		void run(QString cmd);
		void stop();
		void exit();

	signals:
		void finished();
		void error(QString title, QString description);

	private slots:
	
	private:
		int verify_knownhost();
		ssh_session session;
		QString buf;
		QString currentCmd;
		bool connected;

};
