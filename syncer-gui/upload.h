#include <QWizard>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QProgressDialog>
#include <QSettings>
#include <QHostInfo>
#include <QDebug>
#include "ui_initwizard.h"

#include "../shared/runner.h"
#include "../shared/remoteshellrunner.h"

class Init : public QWizard , private Ui::initWizard {
	Q_OBJECT
	public:
		Init(QWidget* parent = 0);
		~Init();

		bool validateCurrentPage();
	private slots:
		void on_folderSelectButton_clicked();
		//void onCurrentIdChanged(int id);
	private:
		void initializePage(int id);
		void accept();

		void initializeComputerPage();
		void initializeServerPage();
		void initializeSharesPage();

		bool validateComputerPage();
		bool validateServerPage();
		bool validateSharesPage();

		bool getUserInfo();

		bool sshKeyGen();
		bool pubKeyAuthorized();
		bool sendPubKey();
		bool gitClone(QString localFolder, QString username, QString host, int port, QString shareName);

		bool setupShare();

		RemoteShellRunner* rsh;

};
