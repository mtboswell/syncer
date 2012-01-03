TEMPLATE = app
TARGET = syncer-undelete

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
mac:target.path = /Applications/syncer
unix:INSTALLS += target

DEPENDPATH += . 
INCLUDEPATH += . 

# Input
HEADERS += undelete.h
FORMS += undelete.ui
SOURCES += main.cpp undelete.cpp
