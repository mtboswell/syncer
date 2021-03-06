TEMPLATE = app
TARGET = syncer

CONFIG(debug, debug|release){
	message("Debug")
	DESTDIR = debug
	OBJECTS_DIR = debug
	MOC_DIR = debug
}else{
	message("Release")
	DESTDIR = ../bin
	OBJECTS_DIR = release
	MOC_DIR = release
}

# documentation.path = /usr/local/syncer/doc
# documentation.files = doc/*
# INSTALLS += documentation
unix:target.path = /usr/local/bin
macx:target.path = /Applications/syncer
unix|macx:INSTALLS += target

DEPENDPATH += .
INCLUDEPATH += . 

# Input
HEADERS += gitlauncher.h
SOURCES += gitlauncher.cpp main.cpp

# Shared
HEADERS += ../shared/runner.h
SOURCES += ../shared/runner.cpp
