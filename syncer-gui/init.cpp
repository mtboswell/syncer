#include "init.h"

Init::Init(QWidget* parent):QWizard(parent){
	setupUi(this);

	rsh = new RemoteShellRunner();

	if(!getUserInfo()) this->reject();

}

Init::~Init(){
	delete rsh;
}

bool Init::getUserInfo(){

	RunResult configRes = Runner::run("git config --global --get user.name");

	QString name = configRes.stdOut;

	if(name.isEmpty()){
		name = QHostInfo::localHostName();
	}

	nameField->setText(name);

	return true;
}

bool Init::validateCurrentPage(){
	switch(currentId()){
		case 0:
			return validateComputerPage();
			break;
		case 1:
			return validateServerPage();
			break;
		case 2:
			return validateSharesPage();
			break;
		default:
			qDebug() << "Error: Invalid page ID";
			return false;
			break;
	}
	return false;
}


bool Init::validateComputerPage(){
	QString name = nameField->text().simplified();
	QString localFolder = folderField->text();

	if(name.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a computer name!");
		return false;
	}
	if(localFolder.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a folder to put the shared folder in!");
		return false;
	}

	RunResult configRes = Runner::run("git config --global user.name \"" + name + "\"");
	if(configRes.status) {
		QMessageBox::information (this, "Settings failed", "Could not set user info, please try again.");
		return false;
	}
}

bool Init::validateServerPage(){
	QString host = hostField->text().simplified();
	int port = portField->value();
	QString username = usernameField->text().simplified();
	QString password = passwordField->text().trimmed();
	
	if(!port) port = 22;

	if(host.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a host!");
		return false;
	}
	if(username.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a username!");
		return false;
	}
	if(password.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a password!");
		return false;
	}


	qDebug() << "Connecting to remote host";

	if(!rsh->connect(host, username, password, port)){
		QMessageBox::critical (this, "Error", "Unable to connect to server!");
		return false;
	}

	qDebug() << "Generating ssh keys";

	if(!sshKeyGen()){
		QMessageBox::critical (this, "Error", "Unable to generate private key!");
		return false;
	}

	qDebug() << "Checking if authorized on server";

	if(!pubKeyAuthorized()){
		qDebug() << "Sending key to server";
		if(!sendPubKey()){
			QMessageBox::critical (this, "Error", "Unable to send public key to server!");
			return false;
		}
	}else qDebug() << "Pub key auth already set up.";

	return true;
}

bool Init::validateSharesPage(){
	return true;
}

void Init::initializePage(int id){
	switch(id){
		case 0:
			initializeComputerPage();
			break;
		case 1:
			initializeServerPage();
			break;
		case 2:
			initializeSharesPage();
			break;
		default:
			qDebug() << "Error: Invalid page ID";
			break;
	}

}

void Init::initializeComputerPage(){

}
void Init::initializeServerPage(){

}
void Init::initializeSharesPage(){

	qDebug() << "Initializing shares page";

	// get shares list
	// find . | grep "/\\.git/" | sed "s/\(.*\)\.git.*/\1/" | sort -u

	if(!rsh->runToEnd("find . | grep \"/\\.git/\" | sed \"s/\\(.*\\)\\.git.*/\1/\" | sort -u")){
		QMessageBox::critical (this, "Error", "Unable to find shares on server!");
		return;
	}
	QStringList shares = rsh->result().split('\n');

	qDebug() << "Found shares" << shares;

	sharesTreeWidget->clear();

	QList<QTreeWidgetItem *> items;

	foreach(QString share, shares){
		QStringList item;
		item << share;

		QTreeWidgetItem* treeItem = new QTreeWidgetItem((QTreeWidget*) 0, item);

		treeItem->setFlags(Qt::ItemIsUserCheckable);
		treeItem->setCheckState(0, Qt::Unchecked);

		items.append(treeItem);
	}

	sharesTreeWidget->insertTopLevelItems(0, items);

}




void Init::accept(){

	QProgressDialog progress("Connecting to server...", "Cancel", 0, 5, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(0);
	progress.setLabelText("Generating SSH keys");

	// get shares from sharesTreeWidget

	// for each share, check to see if we already have it in settings and on filesystem

		// if not on filesystem, then clone the share

		// if not in settings, then add to settings list

/*
	QString localDir = localFolder + QDir::separator() + shareName;

	// Initial synchronization
	if(!gitClone(localFolder, username, host, port, shareName)) return false;


	// Check to see if localDir exists (created by clone)
	QDir localRepo(localDir);
	if(!localRepo.exists()){
		QMessageBox::critical (this, "Error", "Initial synchronization error");
		return false;
	}

	// add to settings

	QSettings* settings = new QSettings("MiBoSoft", "Syncer");
	QStringList dirs = settings->value("syncDirs").toStringList();

	dirs << localDir;

	settings->setValue("syncDirs", dirs);
	*/
}


bool Init::sshKeyGen(){

	//if not exists '~/.ssh/id_rsa.pub' then ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
	QFile pubKeyFile(QDir::homePath() + "/.ssh/id_rsa.pub");
	if(!pubKeyFile.exists()){
		RunResult keygenRes = Runner::run("ssh-keygen -t rsa -N \"\" -f " + QDir::homePath() + "/.ssh/id_rsa");
		if(keygenRes.status) {
			QMessageBox::critical (this, "Error", "ssh-keygen did not finish properly!");
			qDebug() << "ssh-keygen out:" << keygenRes.stdOut << keygenRes.stdErr;
			return false;
		}
	}else{
		qDebug() << "Skipping keygen";
	}

	return true;

}

bool Init::pubKeyAuthorized(){
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

bool Init::sendPubKey(){
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

bool Init::gitClone(QString localFolder, QString username, QString host, int port, QString shareName){

	//git clone ssh://user@host:port/~/share /path/to/share (should not need anything)

	Runner::setWorkingDirectory(localFolder);

	// remote leading slashes
	while(shareName.startsWith("/")) shareName = shareName.right(shareName.size() - 1);

	RunResult cloneRes = Runner::run("git clone ssh://" + username + "@" + host + ":" + QString::number(port)
				+ "/~/" + shareName + " " + shareName);

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

void Init::on_folderSelectButton_clicked(){
	QString path = QFileDialog::getExistingDirectory(this, "Open Shared Folder Base Directory",
			QDir::homePath(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	folderField->setText(path);
}
