#include "ui_restore.h"
#include <QFileSystemModel>
#include <QProcess>
#include <QDateTime>
#include <QDebug>

class Restore : public QWidget, private Ui::RestoreWidget {
		Q_OBJECT
	public:
		Restore(QString path, QWidget *parent = 0);
	private slots:
		void on_restoreButton_clicked();
		void on_upButton_clicked();
		void on_goButton_clicked();
		void fileSelected(QModelIndex index);
		void selectExpanded(QModelIndex index);
		void onDirLoaded(QString path);

	private:
		QFileSystemModel *files;
		QString git;
};
