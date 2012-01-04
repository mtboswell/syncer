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
HEADERS += restore.h
FORMS += restore.ui
SOURCES += main.cpp restore.cpp
