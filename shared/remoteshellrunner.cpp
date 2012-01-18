#include "remoteshellrunner.h"

RemoteShellRunner::RemoteShellRunner(QString host, QString username, QString password, int port){

	connected = false; 

	int rc;

	// Open session and set options
	session = ssh_new();
	if (session == NULL){
		qDebug("Unable to open ssh session");
		return;
	}

	QByteArray hostarray = host.toLatin1();
	const char *hostchar = hostarray.data();
	ssh_options_set(session, SSH_OPTIONS_HOST, hostchar);
	ssh_options_set(session, SSH_OPTIONS_PORT, &port);

	// Connect to server
	rc = ssh_connect(session);
	if (rc != SSH_OK)
	{
		qDebug() << "Error connecting to " << hostchar << ssh_get_error(session);
		ssh_free(session);
		return;
	}

	// Verify the server's identity
	// For the source code of verify_knowhost(), check previous example
	if (verify_knownhost(session) < 0)
	{
		ssh_disconnect(session);
		ssh_free(session);
		return;
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
		return;
	}

	connected = true;

}

RemoteShellRunner::~RemoteShellRunner(){
	ssh_disconnect(session);
	ssh_free(session);
}

void RemoteShellRunner::run(QString cmd){

	ssh_channel channel;
	char buffer[256];
	int nbytes;

	channel = ssh_channel_new(session);
	if (channel == NULL){
		return;//SSH_ERROR;
	}

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		return;//rc;
	}

	rc = ssh_channel_request_exec(channel, (char *) cmd.toLatin1().data());
	if (rc != SSH_OK)
	{
		qDebug() << "Error running command " << cmd << ssh_get_error(session);
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return;//rc;
	}

	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0){
		// writes to STDOUT
		if (write(1, buffer, nbytes) != nbytes){
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			return;//SSH_ERROR;
		}
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}

	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return;//SSH_ERROR;
	}

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
}



int RemoteShellRunner::verify_knownhost(){
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