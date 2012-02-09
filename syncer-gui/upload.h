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
#include "ui_uploadwizard.h"

#include "../shared/runner.h"
#include "../shared/remoteshellrunner.h"

class Upload : public QWizard , private Ui::uploadWizard {
	Q_OBJECT
	public:
		Upload(QWidget* parent = 0);
		~Upload();

		bool validateCurrentPage();
	private slots:
		void on_folderSelectButton_clicked();
		//void onCurrentIdChanged(int id);
	private:
		void initializePage(int id);
		void accept();

		void initializeComputerPage();
		void initializeServerPage();

		bool validateComputerPage();
		bool validateServerPage();

		bool getUserInfo();

		bool sshKeyGen();
		bool pubKeyAuthorized();
		bool sendPubKey();
		bool gitPush(QString localFolder, QString username, QString host, int port, QString shareName);

		bool setupShare();

		RemoteShellRunner* rsh;

};
