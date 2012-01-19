TEMPLATE = app
TARGET = syncer-gui

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
	INCLUDEPATH += libssh
	LIBS += -Llibssh/lib -llibssh
}

unix|macx:LIBS += -lssh

SOURCES += main.cpp\
    syncerlauncher.cpp \
    init.cpp \
    ../shared/shellrunner.cpp \
    ../shared/remoteshellrunner.cpp

HEADERS  += \
    syncerlauncher.h \
    init.h \
    ../shared/shellrunner.h \
    ../shared/remoteshellrunner.h

FORMS    += initwizard.ui

RC_FILE = \
    syncer-gui.rc

RESOURCES += \
    icons.qrc
