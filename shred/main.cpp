#include <QApplication>
#include "undelete.h"

int main(int argc, char** argv){
	QApplication app(argc,argv);
	QStringList args = app.arguments();
	args.removeFirst();
	
	if(args.size() == 0) return 1;

	Undelete undelete(args[0]);
	undelete.show();

	return app.exec();
}
