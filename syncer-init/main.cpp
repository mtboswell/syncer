#include <QApplication>
#include "init.h"


int main(int argc, char *argv[]){
	QApplication a(argc, argv);

	Init* init = new Init;

	init->show();

	return a.exec();
}
