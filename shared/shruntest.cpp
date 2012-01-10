#include "shellrunner.h"
#include <QDebug>


int main(int argc, char** argv){
	
	ShellRunner shrun;

	QString dir = shrun.run("ls", "shared");

	qDebug() << dir;

	shrun.exit();

	return 0;
	
}
