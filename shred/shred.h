#include "ui_shred.h"
#include <QFileSystemModel>
#include <QProcess>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

class Shred : public QWidget, private Ui::ShredWidget {
	Q_OBJECT
	public:
		Shred(QString path, QWidget *parent = 0);
	private slots:
		void on_shredButton_clicked();
		void fileSelected(QListWidgetItem* item);
	private:
		QString selectedFilePath;
		QProcess* gitproc;
		QString git;
};
