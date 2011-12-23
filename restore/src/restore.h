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
		void on_pushButton_clicked();
		void fileSelected(QModelIndex index);

	private:
        QFileSystemModel *files;
        QString git;
};
