#include <libssh/libssh.h>
#include <QObject>
#include <QString>
#include <QBuffer>
#include <QRegExp>
#include <QTime>
#include <QDebug>

class RemoteShellRunner : public QObject {
	Q_OBJECT
	public:
		RemoteShellRunner();
		RemoteShellRunner(QString host, QString username, QString password, int port = 22);
		~RemoteShellRunner();

		bool isConnected();
		bool connect(QString host, QString username, QString password, int port = 22);

		bool run(QString cmd);
		bool runToEnd(QString cmd, int timeout = 5000);
		bool run(QString cmd, QString lookFor);


		bool expect(QString lookFor, int timeout = 5000);
		bool expectRegExp(QString lookFor, int timeout = 5000);
		bool expectEnd(int timeout = 5000);

		bool have(QString lookFor);
		QString result();

		bool cd(QString path);

		enum CmdState {Stopped, Running} state;

	public slots:
		void stop();
		void exit();

	signals:
		void finished();
		void error(QString title, QString description);

	private slots:
	
	private:
		bool read();

		int verify_knownhost();

		ssh_session session;
		ssh_channel channel;
		bool connected;

		QString currentCmd;
		QTime cmdTimer;

		QBuffer buf;

};
