#ifndef SYNCERLAUNCHER_H
#define SYNCERLAUNCHER_H

#include <QObject>
#include <QProcess>
#include <QMap>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QDebug>
#include <QFileDialog>
#include <QDir>

class SyncerLauncher : public QObject
{
		Q_OBJECT
	public:
		explicit SyncerLauncher(QObject *parent = 0);
		
	signals:
		void quit();
		
	public slots:
		void addPath();

	private slots:
		void doAction(QString action);
		void start(QString path);
		void stop(QString path);
		void remove(QString path);

	private:
		QAction* findMenuItem(QMenu* menu, QString item);
		QMap<QString, QProcess*> syncers;
		QString syncerPath;
		QSystemTrayIcon* trayIcon;
		QStringList syncDirs;
		QMenu* syncMenu;
		QSignalMapper* menuMapper;
		QMap<QString, QMenu*> dirMenus;

};

#endif // SYNCERLAUNCHER_H
