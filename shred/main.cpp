#include <QApplication>
#include "shred.h"

int main(int argc, char** argv){
	QApplication app(argc,argv);
	QStringList args = app.arguments();
	args.removeFirst();
	
	if(args.size() == 0) return 1;

	Shred shred(args[0]);
	shred.show();

	return app.exec();
}
