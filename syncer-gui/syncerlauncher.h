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
#include <QSettings>
#include <QMessageBox>

#include "init.h"

class SyncerLauncher : public QObject
{
		Q_OBJECT
	public:
		explicit SyncerLauncher(QObject *parent = 0);
		
	signals:
		void quit();
		
	public slots:
		void addPath();
		void addPath(QString dir);
		void refreshMenu();
		void initPath();

	private slots:
		void doAction(QString action);
		void start(QString path);
		void stop(QString path);
		void remove(QString path);
		void restore(QString path);
		void undelete(QString path);
		void shred(QString path);
		void quitAll();
		void readProcOut(const QString & path);

	private:
		QAction* findMenuItem(QMenu* menu, QString item);
		QMap<QString, QProcess*> syncers;
		QString syncerPath;
		QSystemTrayIcon* trayIcon;
		QStringList syncDirs;
		QMenu* syncMenu;
		QSignalMapper* menuMapper;
		QSignalMapper* procOutMapper;
		QMap<QString, QMenu*> dirMenus;
		QSettings* settings;

};

#endif // SYNCERLAUNCHER_H
