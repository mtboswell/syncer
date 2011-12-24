#ifndef SYNCERLAUNCHER_H
#define SYNCERLAUNCHER_H

#include <QObject>
#include <QProcess>
#include <QMap>

class SyncerLauncher : public QObject
{
		Q_OBJECT
	public:
		explicit SyncerLauncher(QObject *parent = 0);
		
	signals:
		
	public slots:
		void doAction(QString action);

	private slots:
		void start(QString path);
		void stop(QString path);
		void remove(QString path);

	private:
		QMap<QString, QProcess*> syncers;
		QString syncerPath;

};

#endif // SYNCERLAUNCHER_H
