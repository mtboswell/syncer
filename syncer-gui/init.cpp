#include "init.h"

Init::Init(QDialog* parent):QDialog(parent){
	setupUi(this);

	if(!getUserInfo()) this->reject();

}

bool Init::getUserInfo(){
	QProcess* gitProc = new QProcess();
	QStringList gitArgs;
	gitArgs << "config" << "--global" << "--get" << "user.name";

	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return false;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		gitProc->kill();
		return false;
	}
	QString gitOut = gitProc->readAll();

	nameField->setText(gitOut);

	gitArgs.clear();
	gitArgs << "config" << "--global" << "--get" << "user.email";

	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return false;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		gitProc->kill();
		return false;
	}
	gitOut = gitProc->readAll();

	emailField->setText(gitOut);
	return true;
}

void Init::accept(){

	if(!setUserInfo()){
		QMessageBox::information (this, "Settings failed", "Could not set user info, please try again.");
		return;
	}

	if(setupShare()) {
		QMessageBox::information (this, "Settings updated", "Share settings have been updated sucessfully.");
		QDialog::accept();
	}else{
		QMessageBox::information (this, "Settings failed", "Something went wrong, please try again.");
	}

}

bool Init::setUserInfo(){
	QString name = nameField->text().simplified();
	QString email = emailField->text().simplified();


	QProcess* gitProc = new QProcess();
	QStringList gitArgs;

	gitArgs << "config" << "--global" << "user.name" << name;
	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return false;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		gitProc->kill();
		return false;
	}
	QString gitOut = gitProc->readAll();

	gitArgs.clear();
	gitArgs << "config" << "--global" << "user.email" << email;
	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return false;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		gitProc->kill();
		return false;
	}
	gitOut = gitProc->readAll();

	return true;
}

bool Init::sshKeyGen(){

	//if not exists '~/.ssh/id_rsa.pub' then ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
	QFile pubKeyFile(QDir::homePath() + "/.ssh/id_rsa.pub");
	if(!pubKeyFile.exists()){
		QProcess* sshKeyGen = new QProcess();
		QStringList sshKeyGenArgs;
		sshKeyGenArgs << "-t" << "rsa" << "-N" << "" << "-f" << QDir::homePath() + "/.ssh/id_rsa";

		sshKeyGen->start("ssh-keygen", sshKeyGenArgs);
		if(!sshKeyGen->waitForStarted()) {
			qDebug() << "Error: Could not start ssh-keygen";
			QMessageBox::critical (this, "Error", "Could not start ssh-keygen!");
			return false;
		}
		if(!sshKeyGen->waitForFinished()) {
			qDebug() << "Error: ssh-keygen did not finish";
			QMessageBox::critical (this, "Error", "ssh-keygen did not finish properly!");
			sshKeyGen->kill();
			return false;
		}
		QString sshKeyGenOut = sshKeyGen->readAll();
		qDebug() << "ssh-keygen out:\n" << sshKeyGenOut;
	}else{
		qDebug() << "Skipping keygen";
	}

	return true;

}

bool Init::sshKeySend(QString host, int port, QString username, QString password){
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

	ssh_session session;
	int rc;

	// Open session and set options
	session = ssh_new();
	if (session == NULL){
		qDebug("Unable to open ssh session");
		QMessageBox::critical (this, "Error", "Could not open ssh session");
		return false;
	}

	//ssh_options_set(session, SSH_OPTIONS_HOST, "localhost");
	//QByteArray hostchar((const char *) host.data(), host.length());

	QByteArray hostarray = host.toLatin1();
	const char *hostchar = hostarray.data();
	//qDebug() << hostchar[0] << hostchar [1];
	//qDebug() << "Size: " << hostchar.size();
	ssh_options_set(session, SSH_OPTIONS_HOST, hostchar);
	ssh_options_set(session, SSH_OPTIONS_PORT, &port);

	// Connect to server
	rc = ssh_connect(session);
	if (rc != SSH_OK)
	{
		qDebug() << "Error connecting to " << hostchar << ssh_get_error(session);
		QMessageBox::critical (this, "Error", QString("Could not connect to server: ") + hostchar + "\n" + ssh_get_error(session));
		ssh_free(session);
		return false;
	}

	// Verify the server's identity
	// For the source code of verify_knowhost(), check previous example
	if (verify_knownhost(session) < 0)
	{
		QMessageBox::critical (this, "Error", "Could not verify server ID");
		ssh_disconnect(session);
		ssh_free(session);
		return false;
	}

	// Authenticate ourselves
	//password = getpass("Password: ");
	QByteArray userArray = username.toLatin1();
	const char *userchar = userArray.data();
	QByteArray passArray = password.toLatin1();
	const char *passchar = passArray.data();

	rc = ssh_userauth_password(session, userchar, passchar);
	if (rc != SSH_AUTH_SUCCESS)
	{
		qDebug() << "Error authenticating with password:" << ssh_get_error(session);
		QMessageBox::critical (this, "Error", "Bad username or password.");
		ssh_disconnect(session);
		ssh_free(session);
		return false;
	}

	ssh_channel channel;
	char buffer[256];
	int nbytes;

	channel = ssh_channel_new(session);
	if (channel == NULL){
		QMessageBox::critical (this, "Error", "Could not open channel.");
		return false;//SSH_ERROR;
	}

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		QMessageBox::critical (this, "Error", "Could not open channel.");
		return false;//rc;
	}

	rc = ssh_channel_request_exec(channel, (char *) QString("mkdir ~/.ssh").toLatin1().data());
	if (rc != SSH_OK)
	{
		qDebug() << "Error running command mkdir:" << ssh_get_error(session);
		QMessageBox::critical (this, "Error", "Could not create remote directory.");
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return false;//rc;
	}

	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0){
		if (write(1, buffer, nbytes) != nbytes){
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			QMessageBox::critical (this, "Error", "Command read error");
			return false;//SSH_ERROR;
		}
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}

	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		QMessageBox::critical (this, "Error", "Command read error");
		return false;//SSH_ERROR;
	}

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);

	channel = ssh_channel_new(session);
	if (channel == NULL){
		QMessageBox::critical (this, "Error", "Could not open channel");
		return false;//SSH_ERROR;
	}

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		QMessageBox::critical (this, "Error", "Could not open channel.");
		return false;//rc;
	}

	QString echoCmd("echo \"" + key.simplified() + "\" >> ~/.ssh/authorized_keys");
	rc = ssh_channel_request_exec(channel, (char *) echoCmd.toLatin1().data());
	if (rc != SSH_OK)
	{
		qDebug() << "Error running command:" << echoCmd << " : " << ssh_get_error(session);
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		QMessageBox::critical (this, "Error", "Could not update key on server.");
		return false;//rc;
	}

	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0){
		if (write(1, buffer, nbytes) != nbytes){
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			QMessageBox::critical (this, "Error", "Command read error");
			return false;//SSH_ERROR;
		}
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}

	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		QMessageBox::critical (this, "Error", "Command read error");
		return false;//SSH_ERROR;
	}



	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);



	ssh_disconnect(session);
	ssh_free(session);

	return true;
}

bool Init::gitClone(QString localFolder, QString username, QString host, int port, QString shareName){

	//git clone ssh://user@host:port/~/share /path/to/share (should not need anything)
/*
		QProcess *gitproc = new QProcess();
	gitproc->setProcessChannelMode(QProcess::MergedChannels);
	QString git("git");


	QStringList cloneArgs;
	cloneArgs << "clone" << "ssh://" + username + "@" + host + ":" + QString::number(port)
				+ "/~/" + shareName << localDir;

	gitproc->start(git, cloneArgs);
	if(!gitproc->waitForStarted()){
		qDebug() << "Error: git did not start (" << gitproc->error() << ")";
		QMessageBox::critical (this, "Error", "Git did not start");
		return false;
	}
	*/
	/*
	gitproc->waitForReadyRead();
	QString gitOut = gitproc->readAll();
	QMessageBox::information(this, "First:", gitOut);

	gitproc->waitForReadyRead();
	gitOut = gitproc->readAll();
	QMessageBox::information(this, "Second:", gitOut);

	gitproc->write("yes\n");
	*/
/*
	if(!gitproc->waitForFinished()){
		qDebug() << "Error: git did not finish (" << gitproc->error() << ")";
		QMessageBox::critical (this, "Error", "Git did not finish");
		return false;
	}
	*/


	QProcess* shProc = new QProcess();
	shProc->setProcessChannelMode(QProcess::MergedChannels);

	shProc->start("sh", QStringList() << "--login" << "-i");
	if(!shProc->waitForStarted()){
		qDebug() << "Error: sh did not start (" << shProc->error() << ")";
		QMessageBox::critical (this, "Error", "sh did not start: " + shProc->error());
		return false;
	}

//	shProc->waitForReadyRead();
//	QString shOut = shProc->readAll();
//	QMessageBox::information(this, "Sh:", shOut);

	shProc->write("export PATH=$PATH:$PWD\n");

	shProc->setWorkingDirectory(localFolder);
	shProc->write(QString("git clone ssh://" + username + "@" + host + ":" + QString::number(port)
				+ "/~/" + shareName).toLatin1());
	shProc->write("\n");

//	shProc->waitForReadyRead();

	shProc->write("exit\n");

	shProc->closeWriteChannel();

	if(!shProc->waitForFinished()){
		qDebug() << "Error: sh did not finish (" << shProc->error() << ")";
		QMessageBox::critical (this, "Error", "sh did not finish");
		shProc->kill();
		return false;
	}
	QString gitOut = shProc->readAll();
	//QMessageBox::information(this, "Git:", gitOut);

	// todo: handle clone errors
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

bool Init::setupShare(){
	QTextStream out(stdout);


	QString host = hostField->text().simplified();
	int port = portField->text().toInt();
	QString username = usernameField->text().simplified();
	QString password = passwordField->text().trimmed();
	QString shareName = sharenameField->text().simplified();
	QString localFolder = folderField->text();

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
	if(shareName.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a share name!");
		return false;
	}
	if(localFolder.isEmpty()){
		QMessageBox::critical (this, "Error", "You must specify a folder to put the shared folder in!");
		return false;
	}

	QProgressDialog progress("Connecting to server...", "Cancel", 0, 5, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(0);
	progress.setLabelText("Generating SSH keys");

	// Generate key pair
	if(!sshKeyGen()) return false;

	progress.setValue(1);
	if(progress.wasCanceled()) return false;
	progress.setLabelText("Sending public key.");

	// Send public key to server
	if(!sshKeySend(host, port, username, password)) return false;

	progress.setValue(2);
	if(progress.wasCanceled()) return false;
	progress.setLabelText("Downloading shared folder...");

	QString localDir = localFolder + QDir::separator() + shareName;

	// Initial synchronization
	if(!gitClone(localFolder, username, host, port, shareName)) return false;

	progress.setValue(3);
	if(progress.wasCanceled()) return false;
	progress.setLabelText("Checking local files");

	// Check to see if localDir exists (created by clone)
	QDir localRepo(localDir);
	if(!localRepo.exists()){
		QMessageBox::critical (this, "Error", "Initial synchronization error");
		return false;
	}

	progress.setValue(4);
	if(progress.wasCanceled()) return false;
	progress.setLabelText("Adding to sync list");

	// add to settings

	QSettings* settings = new QSettings("MiBoSoft", "Syncer");
	QStringList dirs = settings->value("syncDirs").toStringList();

	dirs << localDir;

	settings->setValue("syncDirs", dirs);

	// done
	progress.setValue(5);

	return true;
}

int Init::verify_knownhost(ssh_session session){
	int state, hlen;
	unsigned char *hash = NULL;
	char *hexa;
	//char buf[10];

	state = ssh_is_server_known(session);

	hlen = ssh_get_pubkey_hash(session, &hash);
	if (hlen < 0)
		return -1;

	switch (state)
	{
		case SSH_SERVER_KNOWN_OK:
			break; 

		case SSH_SERVER_KNOWN_CHANGED:
			qDebug( "Host key for server changed: it is now:\n");
			ssh_print_hexa("Public key hash", hash, hlen);
			qDebug("For security reasons, connection will be stopped\n");
			free(hash);
			return -1;

		case SSH_SERVER_FOUND_OTHER:
			qDebug("The host key for this server was not found but an other type of key exists.");
			qDebug("An attacker might change the default server key to confuse your client into thinking the key does not exist");
			qDebug() << hash;
			free(hash);
			return -1;

		case SSH_SERVER_FILE_NOT_FOUND:
			qDebug("Could not find known host file.");
			qDebug("If you accept the host key here, the file will be automatically created.");

		case SSH_SERVER_NOT_KNOWN:
			hexa = ssh_get_hexa(hash, hlen);
			qDebug("The server is unknown. Do you trust the host key?");
			qDebug() << "Public key hash:" <<  hexa;
			free(hexa);
			if (ssh_write_knownhost(session) < 0)
			{
				qDebug() << "Error writing known host";
				free(hash);
				return -1;
			}
			break;

		case SSH_SERVER_ERROR:
			qDebug() << "Error" << ssh_get_error(session);
			free(hash);
			return -1;
	}

	free(hash);
	return 0;
}

void Init::on_folderSelectButton_clicked(){
	QString path = QFileDialog::getExistingDirectory(this, "Open Shared Folder Base Directory",
			QDir::homePath(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	folderField->setText(path);
}
