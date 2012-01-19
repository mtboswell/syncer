#include "remoteshellrunner.h"
#include <QDebug>


int main(int argc, char** argv){
	
	RemoteShellRunner rsh("localhost", "gittest", "git", 22);
	if(!rsh.isConnected()) return 1;

	if(!rsh.cd("~")){
		qDebug() << "CD failed!";
		return 1;
	}

	if(!rsh.run("ls", "share")){
		qDebug() << "Did not find share in output";
		qDebug() << rsh.result();
	}else{
		qDebug() << "Found share!";
	}

	if(!rsh.run("passwd")) qDebug() << "Could not passwd";
	else qDebug() << "passwd succeeded with output:" << rsh.result();

	rsh.exit();

	return 0;
	
}
