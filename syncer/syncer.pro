TEMPLATE = app
<<<<<<< HEAD

CONFIG(debug, debug|release){
	message("Debug")
	TARGET = syncer
=======
TARGET = syncer

CONFIG(debug, debug|release){
	message("Debug")
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581
	DESTDIR = debug
	OBJECTS_DIR = debug
	MOC_DIR = debug
}else{
	message("Release")
<<<<<<< HEAD
	TARGET = syncer
=======
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581
	DESTDIR = ../bin
	OBJECTS_DIR = release
	MOC_DIR = release
}

<<<<<<< HEAD
# documentation.path = /usr/local/program/doc
# documentation.files = docs/*
# INSTALLS += documentation
unix:target.path = /usr/bin
#target.path = $$[QT_INSTALL_BINS]
unix:INSTALLS += target
=======
# documentation.path = /usr/local/syncer/doc
# documentation.files = doc/*
# INSTALLS += documentation
unix:target.path = /usr/local/bin
macx:target.path = /Applications/syncer
unix|macx:INSTALLS += target
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581

DEPENDPATH += .
INCLUDEPATH += . 

# Input
HEADERS += gitlauncher.h
SOURCES += gitlauncher.cpp main.cpp
