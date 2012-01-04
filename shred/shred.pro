TEMPLATE = app
TARGET = syncer-shred

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

unix:target.path = /usr/local/bin
macx:target.path = /Applications/syncer
unix|macx:INSTALLS += target

DEPENDPATH += . 
INCLUDEPATH += . 

# Input
HEADERS += shred.h
FORMS += shred.ui
SOURCES += main.cpp shred.cpp
