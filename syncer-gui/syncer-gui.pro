TEMPLATE = app
TARGET = syncer-gui

QT += network
CONFIG += QT

CONFIG(debug, debug|release){
	message("Debug")
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

win32 {
	DEPENDPATH += .
	INCLUDEPATH += . libssh
	LIBS += -Llibssh/lib -llibssh
}

unix|macx:LIBS += -lssh

SOURCES += main.cpp\
    syncerlauncher.cpp \
    init.cpp \
    ../shared/runner.cpp \
    ../shared/remoteshellrunner.cpp

HEADERS  += \
    syncerlauncher.h \
    init.h \
    ../shared/runner.h \
    ../shared/remoteshellrunner.h

FORMS    += initwizard.ui

RC_FILE = \
    syncer-gui.rc

RESOURCES += \
    icons.qrc
