#include "shellrunner.h"
#include <QDebug>


int main(int argc, char** argv){
	
	ShellRunner shrun;

	shrun.cd("/home/micah/tmp");

	if(!shrun.run("ls", "share")){
		qDebug() << "Did not find share in output";
		qDebug() << shrun.result();
	}else{
		qDebug() << "Found share!";
	}

	shrun.exit();

	return 0;
	
}
