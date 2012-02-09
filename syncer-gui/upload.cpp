#include "upload.h"

Upload::Upload(QWidget* parent):QWizard(parent){
	setupUi(this);

	rsh = new RemoteShellRunner();

	if(!getUserInfo()) this->reject();

}

Upload::~Upload(){
	delete rsh;
}

bool Upload::getUserInfo(){

	RunResult configRes = Runner::run("git config --global --get user.name");

	QString name = configRes.stdOut;

	if(name.isEmpty()){
		name = QHostInfo::localHostName();
	}

	nameField->setText(name);

	return true;
}

bool Upload::validateCurrentPage(){
	switch(currentId()){
		case 0:
			return validateComputerPage();
			break;
		case 1:
			return validateServerPage();
			break;
		default:
			qDebug() << "Error: Invalid page ID";
			return false;
			break;
	}
	return false;
}


bool Upload::validateComputerPage(){
	QString name = nameField->text().simplified();
	QString localFolder = folderField->text();
	QString shareName = shareNameField->text().simplified();

	if(name.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a computer name!");
		return false;
	}
	if(localFolder.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a folder to put the shared folder in!");
		return false;
	}
	if(shareName.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a share name!");
		return false;
	}

	RunResult configRes = Runner::run("git config --global user.name \"" + name + "\"");
	if(configRes.status) {
		QMessageBox::information (this, "Settings failed", "Could not set user info, please try again.");
		return false;
	}

	return true;
}

bool Upload::validateServerPage(){
	QString host = hostField->text().simplified();
	int port = portField->value();
	QString username = usernameField->text().simplified();
	
	if(!port) port = 22;

	if(host.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a host!");
		return false;
	}
	if(username.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a username!");
		return false;
	}

	bool usingKey = false;

	//qDebug() << "Connecting to remote host";

	if(!rsh->connect(host, username, port)){
		bool ok;
		QString password = QInputDialog::getText(this, "Server password", "Password:", QLineEdit::Password, "", &ok);
		if(!ok || !rsh->connect(host, username, password, port)){
			QMessageBox::critical (this, "Error", "Unable to connect to server!");
			return false;
		}
	}else{
		usingKey = true;
	}

	if(!usingKey){
		//qDebug() << "Generating ssh keys";

		if(!sshKeyGen()){
			QMessageBox::critical (this, "Error", "Unable to generate private key!");
			return false;
		}

		//qDebug() << "Checking if authorized on server";

		if(!pubKeyAuthorized()){
			//qDebug() << "Sending key to server";
			if(!sendPubKey()){
				QMessageBox::critical (this, "Error", "Unable to send public key to server!");
				return false;
			}
		}else qDebug() << "Pub key auth already set up.";
	}

	return true;
}

void Upload::initializePage(int id){
	switch(id){
		case 0:
			initializeComputerPage();
			break;
		case 1:
			initializeServerPage();
			break;
		default:
			qDebug() << "Error: Invalid page ID";
			break;
	}

}

void Upload::initializeComputerPage(){

}
void Upload::initializeServerPage(){

}

void Upload::accept(){


	// check to see if share exists on server

	// check to see if local folder already has git

	// git init

	// git add all

	// git set remote

	// git push


	this->setCursor(Qt::WaitCursor);

	QProgressDialog progress("Synchronizing from server...", "Cancel", 0, selectedShares.size(), this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(shareCount);
	progress.setLabelText("Synchronizing "+selectedShares.at(shareCount));

	QString localFolder = folderField->text();
	QString host = hostField->text().simplified();
	int port = portField->value();
	QString username = usernameField->text().simplified();
	
	if(!port) port = 22;


	QSettings* settings = new QSettings("MiBoSoft", "Syncer");
	QStringList dirs = settings->value("syncDirs").toStringList();

	QDir localRepo(localDir);

	// if not in settings, then add to settings list
	if(!dirs.contains(localDir))
		dirs << localDir;


	progress.setValue(selectedShares.size());

	settings->setValue("syncDirs", dirs);

	QWizard::accept();

}


bool Upload::sshKeyGen(){

	//if not exists '~/.ssh/id_rsa.pub' then ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
	QFile pubKeyFile(QDir::homePath() + "/.ssh/id_rsa.pub");
	if(!pubKeyFile.exists()){
		RunResult keygenRes = Runner::run("ssh-keygen -t rsa -N \"\" -f " + QDir::homePath() + "/.ssh/id_rsa");
		if(keygenRes.status) {
			QMessageBox::critical (this, "Error", "ssh-keygen did not finish properly!");
			//qDebug() << "ssh-keygen out:" << keygenRes.stdOut << keygenRes.stdErr;
			return false;
		}
	}else{
		qDebug() << "Skipping keygen";
	}

	return true;

}

bool Upload::pubKeyAuthorized(){
	QString key;
	QFile keyFile(QDir::homePath() + "/.ssh/id_rsa.pub");
	if (!keyFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QMessageBox::critical (this, "Error", "Could not open public key.");
		return false;
	}

	while (!keyFile.atEnd()) {
		key += keyFile.readLine();
	}

	if(!rsh->runToEnd("cat ~/.ssh/authorized_keys")){
		QMessageBox::critical (this, "Error", "Could not open server key file.");
		return false;
	}
	QString authorized_keys = rsh->result();

	return authorized_keys.contains(key);
}

bool Upload::sendPubKey(){
	//ssh user@host -p port 'echo "key" >> ~/.ssh/authorized_keys', enter password
	QString key;
	QFile keyFile(QDir::homePath() + "/.ssh/id_rsa.pub");
	if (!keyFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QMessageBox::critical (this, "Error", "Could not open public key.");
		return false;
	}

	while (!keyFile.atEnd()) {
		key += keyFile.readLine();
	}


	if(!rsh->runToEnd("mkdir ~/.ssh")){
		QMessageBox::critical (this, "Error", "Could not create key dir on server.");
		return false;
	}

	QString echoCmd("echo \"" + key.simplified() + "\" >> ~/.ssh/authorized_keys");
	if(!rsh->runToEnd(echoCmd)){
		QMessageBox::critical (this, "Error", "Could not add key to server key list.");
		return false;
	}

	return true;
}

bool Upload::gitClone(QString localFolder, QString username, QString host, int port, QString shareName){

	//git clone ssh://user@host:port/~/share /path/to/share (should not need anything)

	Runner::setWorkingDirectory(localFolder);

	// remote leading slashes
	while(shareName.startsWith("/")) shareName = shareName.right(shareName.size() - 1);

	RunResult cloneRes = Runner::run("git clone ssh://" + username + "@" + host + ":" + QString::number(port)
				+ "/~/" + shareName + " " + shareName, -1);

	if(cloneRes.status){
		QMessageBox::critical (this, "Error", "git clone did not finish");
		return false;
	}

	QString gitOut = cloneRes.stdOut + cloneRes.stdErr;
	//QMessageBox::information(this, "Git:", gitOut);

	if(!gitOut.isEmpty() && !gitOut.contains("Already up-to-date.")){
		if(gitOut.contains("fatal")){
			qDebug() << "Error:" << gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6) << "\n";
			QMessageBox::critical (this, "Error", gitOut.right(gitOut.size() - gitOut.indexOf("fatal:") - 6));
			return false;

		}
		else if(gitOut.contains("Cloning"))
			qDebug() << "Synchronized from server\n";
		else{
			qDebug() << "Unknown Error\n" << gitOut;
			QMessageBox::critical (this, "Unknown git output", gitOut);
			return false;
		}

	}

	return true;
}

void Upload::on_folderSelectButton_clicked(){
	QString path = QFileDialog::getExistingDirectory(this, "Open Shared Folder Base Directory",
			QDir::homePath(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	folderField->setText(path);
}
