#include <QApplication>
#include "undelete.h"

int main(int argc, char** argv){
	QApplication app(argc,argv);
	QStringList args = app.arguments();
	args.removeFirst();

	Undelete undelete(args[0]);
	undelete.show();

	app.exec();
}
