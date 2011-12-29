TEMPLATE = app

CONFIG(debug, debug|release){
	message("Debug")
        TARGET = restore
	DESTDIR = debug
        OBJECTS_DIR = debug
        MOC_DIR = debug
} else {
        TARGET = restore
	DESTDIR = ../bin
        OBJECTS_DIR = release
        MOC_DIR = release
}

unix:target.path = /usr/bin
unix:INSTALLS += target

DEPENDPATH += . 
INCLUDEPATH += . 

# Input
HEADERS += restore.h
FORMS += restore.ui
SOURCES += main.cpp restore.cpp
