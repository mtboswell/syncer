TEMPLATE = app

CONFIG(debug, debug|release){
	message("Debug")
	TARGET = undelete
	DESTDIR = debug
	OBJECTS_DIR = debug
	MOC_DIR = debug
}else{
	message("Release")
	TARGET = undelete
	DESTDIR = ../bin
	OBJECTS_DIR = release
	MOC_DIR = release
}

unix:target.path = /usr/bin
unix:INSTALLS += target

DEPENDPATH += . 
INCLUDEPATH += . 

# Input
HEADERS += undelete.h
FORMS += undelete.ui
SOURCES += main.cpp undelete.cpp
