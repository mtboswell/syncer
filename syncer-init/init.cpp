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

//	setupShare();

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
