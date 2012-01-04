#include <QDialog>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include "ui_init.h"

#include <libssh/libssh.h>

class Init : public QDialog , private Ui::initDialog {
	Q_OBJECT
	public:
		Init(QDialog* parent = 0);
	private slots:
		void accept();
		void on_folderSelectButton_clicked();
	private:
		void getUserInfo();
		void setUserInfo();
		bool setupShare();
		int verify_knownhost(ssh_session session);

};
