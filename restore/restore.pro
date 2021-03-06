TEMPLATE = app

CONFIG(debug, debug|release){
	message("Debug")
        TARGET = syncer-restore
	DESTDIR = debug
        OBJECTS_DIR = debug
        MOC_DIR = debug
} else {
        TARGET = syncer-restore
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
HEADERS += restore.h
FORMS += restore.ui
SOURCES += main.cpp restore.cpp
