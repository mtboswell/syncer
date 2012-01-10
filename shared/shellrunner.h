#include <QProcess>

class ShellRunner : public QObject {
	Q_OBJECT
	public:
		ShellRunner(QString workingDirectory = ".", QString shell = "bash -i");
		~ShellRunner();
		QString expect(QString lookFor);
		QString run(QString cmd, QString lookFor);
		bool have(QString lookFor);

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
