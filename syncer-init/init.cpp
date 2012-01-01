#include "init.h"

Init::Init(QDialog* parent):QDialog(parent){
	setupUi(this);

	getUserInfo();

}

void Init::getUserInfo(){
	QProcess* gitProc = new QProcess();
	QStringList gitArgs;
	gitArgs << "config" << "--global" << "--get" << "user.name";

	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		return;
	}
	QString gitOut = gitProc->readAll();

	nameField->setText(gitOut);

	gitArgs.clear();
	gitArgs << "config" << "--global" << "--get" << "user.email";

	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		return;
	}
	gitOut = gitProc->readAll();

	emailField->setText(gitOut);
}

void Init::accept(){

	setUserInfo();

	if(setupShare())
		QMessageBox::information (this, "Settings updated", "Share settings have been updated sucessfully.");

	QDialog::accept();
}

void Init::setUserInfo(){
	QString name = nameField->text().simplified();
	QString email = emailField->text().simplified();


	QProcess* gitProc = new QProcess();
	QStringList gitArgs;

	gitArgs << "config" << "--global" << "user.name" << name;
	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		return;
	}
	QString gitOut = gitProc->readAll();

	gitArgs.clear();
	gitArgs << "config" << "--global" << "user.email" << email;
	gitProc->start("git", gitArgs);

	if(!gitProc->waitForStarted()) {
		qDebug() << "Error: Could not start git";
		QMessageBox::critical (this, "Error", "Could not start Git!");
		return;
	}
	if(!gitProc->waitForFinished()) {
		qDebug() << "Error: Git did not finish";
		QMessageBox::critical (this, "Error", "Git did not finish properly!");
		return;
	}
	gitOut = gitProc->readAll();

}

bool Init::setupShare(){
	QString host = hostField->text().simplified();
	int port = portField->text().toInt();
	QString username = usernameField->text().simplified();
	QString password = passwordField->text().trimmed();
	QString shareName = sharenameField->text().simplified();
	QString localFolder = folderField->text();


	QTextStream out(stdout);

	//if not exists '~/.ssh/id_rsa.pub' then ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
	QFile pubKeyFile(QDir::homePath() + "/.ssh/id_rsa.pub");
	if(!pubKeyFile.exists()){
		QProcess* sshKeyGen = new QProcess();
		QStringList sshKeyGenArgs;
		sshKeyGenArgs << "-t" << "rsa" << "-N" << "" << "-f" << "~/.ssh/id_rsa";

		sshKeyGen->start("ssh-keygen", sshKeyGenArgs);
		if(!sshKeyGen->waitForStarted()) {
			qDebug() << "Error: Could not start ssh-keygen";
			QMessageBox::critical (this, "Error", "Could not start ssh-keygen!");
			return false;
		}
		if(!sshKeyGen->waitForFinished()) {
			qDebug() << "Error: ssh-keygen did not finish";
			QMessageBox::critical (this, "Error", "ssh-keygen did not finish properly!");
			return false;
		}
		QString sshKeyGenOut = sshKeyGen->readAll();
		qDebug() << "ssh-keygen out:\n" << sshKeyGenOut;
	}else{
		qDebug() << "Skipping keygen";
	}


	//scp -o StrictHostKeyChecking=no -P port ~/.ssh/id_rsa.pub user@host:user_id.pub, enter password

	/*

	QProcess* scp = new QProcess();
	scp->setProcessChannelMode(QProcess::MergedChannels);
	QStringList scpArgs;
	scpArgs << "-o" << "StrictHostKeyChecking=no" << "-P" << QString::number(port)
		<< "~/.ssh/id_rsa.pub" << username + "@" + host + ":" + username + "_id.pub";

	QString scpOut;

	scp->start("scp", scpArgs);
	if(!scp->waitForStarted()) {
		qDebug() << "Error: Could not start scp";
		QMessageBox::critical (this, "Error", "Could not start scp!");
		return false;
	}
	if(!scp->waitForReadyRead()) {
		qDebug() << "Error: Could not get scp output";
		QMessageBox::critical (this, "Error", "Could not get scp output!");
		return false;
	}

	scp->write(QByteArray((char *) password.data()));
	scp->write("\n");

	if(!scp->waitForFinished()) {
		qDebug() << "Error: scp did not finish";
		QMessageBox::critical (this, "Error", "scp did not finish properly!");
		return false;
	}
	scpOut = scp->readAll();
	qDebug() << "scp out:\n" << scpOut;
	*/


	//ssh user@host -p port 'echo "key" >> ~/.ssh/authorized_keys', enter password
	QString key;
	QFile keyFile(QDir::homePath() + "/.ssh/id_rsa.pub");
	if (!keyFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	while (!keyFile.atEnd()) {
		key += keyFile.readLine();
	}

	ssh_session session;
	int rc;

	// Open session and set options
	session = ssh_new();
	if (session == NULL){
		qDebug("Unable to open ssh session");
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
		ssh_free(session);
		return false;
	}

	// Verify the server's identity
	// For the source code of verify_knowhost(), check previous example
	if (verify_knownhost(session) < 0)
	{
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
		ssh_disconnect(session);
		ssh_free(session);
		return false;
	}

	ssh_channel channel;
	char buffer[256];
	unsigned int nbytes;

	channel = ssh_channel_new(session);
	if (channel == NULL)
		return false;//SSH_ERROR;

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		return false;//rc;
	}

	rc = ssh_channel_request_exec(channel, (char *) QString("mkdir ~/.ssh").toLatin1().data());
	if (rc != SSH_OK)
	{
		qDebug() << "Error running command mkdir:" << ssh_get_error(session);
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return false;//rc;
	}

	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0){
		if (write(1, buffer, nbytes) != nbytes){
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			return false;//SSH_ERROR;
		}
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}

	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return false;//SSH_ERROR;
	}

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);


	channel = ssh_channel_new(session);
	if (channel == NULL)
		return false;//SSH_ERROR;

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		return false;//rc;
	}

	QString echoCmd("echo \"" + key.simplified() + "\" >> ~/.ssh/authorized_keys");
	rc = ssh_channel_request_exec(channel, (char *) echoCmd.toLatin1().data());
	if (rc != SSH_OK)
	{
		qDebug() << "Error running command:" << echoCmd << " : " << ssh_get_error(session);
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return false;//rc;
	}

	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0){
		if (write(1, buffer, nbytes) != nbytes){
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			return false;//SSH_ERROR;
		}
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}

	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return false;//SSH_ERROR;
	}



	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);



	ssh_disconnect(session);
	ssh_free(session);


	//git clone ssh://user@host:port/~/share /path/to/share (should not need anything)

        QProcess *gitproc = new QProcess();
	gitproc->setProcessChannelMode(QProcess::MergedChannels);
	QString git("git");

	QString localDir = localFolder + "/" + shareName;

	QStringList cloneArgs;
	cloneArgs << "clone" << "ssh://" + username + "@" + host + ":" + QString::number(port)
				+ "/~/" + shareName << localDir;

	gitproc->start(git, cloneArgs);
	if(!gitproc->waitForStarted()){
		out << "Error: git did not start (" << gitproc->error() << ")";
		return false;
	}
	if(!gitproc->waitForFinished()){
		out << "Error: git did not finish (" << gitproc->error() << ")";
		return false;
	}
	QString gitOut = gitproc->readAll();



	//cd /path/to/share
	gitproc->setWorkingDirectory(localDir);

	//git add .

	QStringList addArgs;
	addArgs << "add" << ".";

	gitproc->start(git, addArgs);
	if(!gitproc->waitForStarted()){
		out << "Error: git did not start (" << gitproc->error() << ")";
		return false;
	}
	if(!gitproc->waitForFinished()){
		out << "Error: git did not finish (" << gitproc->error() << ")";
		return false;
	}
	gitOut = gitproc->readAll();


	//git commit -m "Initial Commit"
	QStringList commitArgs;
	commitArgs << "commit" << "-m" << "Initial Commit for device X";

	gitproc->start(git, commitArgs);
	if(!gitproc->waitForStarted()){
		out << "Error: git did not start (" << gitproc->error() << ")";
		return false;
	}
	if(!gitproc->waitForFinished()){
		out << "Error: git did not finish (" << gitproc->error() << ")";
		return false;
	}
	gitOut = gitproc->readAll();



	//git push origin master
	QStringList pushArgs;
	pushArgs << "push" << "origin" << "master";

	gitproc->start(git, pushArgs);
	if(!gitproc->waitForStarted()){
		out << "Error: git did not start (" << gitproc->error() << ")";
		return false;
	}
	if(!gitproc->waitForFinished()){
		out << "Error: git did not finish (" << gitproc->error() << ")";
		return false;
	}
	gitOut = gitproc->readAll();

	// add to settings

	QSettings* settings = new QSettings("MiBoSoft", "Syncer");
	QStringList dirs = settings->value("syncDirs").toStringList();

	dirs << localDir;

	settings->setValue("syncDirs", dirs);

	// tell syncer-gui to refresh


	return true;
}

int Init::verify_knownhost(ssh_session session){
	int state, hlen;
	unsigned char *hash = NULL;
	char *hexa;
	char buf[10];

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
