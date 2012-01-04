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

LIBS += -lssh

SOURCES += main.cpp\
    syncerlauncher.cpp \
    init.cpp

HEADERS  += \
    syncerlauncher.h \
    init.h

FORMS    += init.ui

OTHER_FILES +=

RESOURCES += \
    icons.qrc
