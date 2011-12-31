#include <QDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include "ui_init.h"

class Init : public QDialog , private Ui::initDialog {
	Q_OBJECT
	public:
		Init(QDialog* parent = 0);
	private slots:
		void accept();
	private:
		void getUserInfo();
		void setUserInfo();

};
