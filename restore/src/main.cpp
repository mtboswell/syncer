#include <QApplication>
#include "restore.h"

int main(int argc, char** argv){
	QApplication app(argc,argv);
	QStringList args = app.arguments();
	args.removeFirst();

	Restore restore(args[0]);
	restore.show();

    app.exec();

    return 0;
}
