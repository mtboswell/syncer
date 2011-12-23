#include "ui_undelete.h"
#include <QFileSystemModel>
#include <QProcess>
#include <QDateTime>
#include <QDebug>

class Undelete : public QWidget, private Ui::UndeleteWidget {
	Q_OBJECT
	public:
		Undelete(QString path, QWidget *parent = 0);
	private slots:
		void on_pushButton_clicked();
		void fileSelected(QListWidgetItem* item);
	private:
		QString selectedFilePath;
		QProcess* gitproc;
		QString git;
};
