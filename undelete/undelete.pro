TEMPLATE = app
<<<<<<< HEAD

CONFIG(debug, debug|release){
	message("Debug")
	TARGET = syncer-undelete
=======
TARGET = syncer-undelete

CONFIG(debug, debug|release){
	message("Debug")
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581
	DESTDIR = debug
	OBJECTS_DIR = debug
	MOC_DIR = debug
}else{
	message("Release")
<<<<<<< HEAD
	TARGET = syncer-undelete
=======
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581
	DESTDIR = ../bin
	OBJECTS_DIR = release
	MOC_DIR = release
}

<<<<<<< HEAD
unix:target.path = /usr/bin
unix:INSTALLS += target
=======
unix:target.path = /usr/local/bin
macx:target.path = /Applications/syncer
unix|macx:INSTALLS += target
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581

DEPENDPATH += . 
INCLUDEPATH += . 

# Input
HEADERS += undelete.h
FORMS += undelete.ui
SOURCES += main.cpp undelete.cpp
