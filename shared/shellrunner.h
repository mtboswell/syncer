#include <QProcess>

class ShellRunner : public QObject {
	Q_OBJECT
	public:
		ShellRunner(QString workingDirectory = ".", QString shell = "bash --noediting --norc --noprofile -i");
		~ShellRunner();
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
		void onReadyReadStdOut();
		void onReadyReadStdErr();
	
	private:
		QProcess* shProc;
		QString buf;
		QString currentCmd;

};
