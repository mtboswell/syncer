TEMPLATE = app

CONFIG(debug, debug|release){
	message("Debug")
	TARGET = syncer
	DESTDIR = debug
	OBJECTS_DIR = debug
	MOC_DIR = debug
}else{
	message("Release")
	TARGET = syncer
	DESTDIR = ../bin
	OBJECTS_DIR = release
	MOC_DIR = release
}

# documentation.path = /usr/local/program/doc
# documentation.files = docs/*
# INSTALLS += documentation
unix:target.path = /usr/bin
#target.path = $$[QT_INSTALL_BINS]
unix:INSTALLS += target

DEPENDPATH += .
INCLUDEPATH += . 

# Input
HEADERS += gitlauncher.h
SOURCES += gitlauncher.cpp main.cpp
