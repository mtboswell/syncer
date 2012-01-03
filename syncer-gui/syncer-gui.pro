TEMPLATE = app

CONFIG(debug, debug|release){
	message("Debug")
	TARGET = syncer-gui
	DESTDIR = debug
        OBJECTS_DIR = debug
        MOC_DIR = debug
}else{
	message("Release")
	TARGET = syncer-gui
	DESTDIR = ../bin
	OBJECTS_DIR = release
	MOC_DIR = release
}

unix:target.path = /usr/local/bin
unix:INSTALLS += target

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
