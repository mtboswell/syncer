#include <QApplication>
#include <QFileDialog>
#include "restore.h"

int main(int argc, char** argv){
	QApplication app(argc,argv);
	QStringList args = app.arguments();
	args.removeFirst();

	QString path;

	if(args.size() == 0)
//        path = QDir::currentPath();
		path = QFileDialog::getExistingDirectory(0, "Open Git Repository Base Directory",
														 QDir::homePath(),
														 QFileDialog::ShowDirsOnly
														 | QFileDialog::DontResolveSymlinks);
	else path = args[0];

	Restore restore(path);
	restore.show();

    app.exec();

    return 0;
}
