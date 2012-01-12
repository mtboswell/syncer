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

win32:INCLUDEPATH += libssh

win32:LIBS += -Llibssh/lib
unix|macx:LIBS += -lssh
win32:LIBS += -llibssh

SOURCES += main.cpp\
    syncerlauncher.cpp \
    init.cpp \
    ../shared/shellrunner.cpp

HEADERS  += \
    syncerlauncher.h \
    init.h \
    ../shared/shellrunner.h

FORMS    += init.ui

RC_FILE = \
    syncer-gui.rc

RESOURCES += \
    icons.qrc
