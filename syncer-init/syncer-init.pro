TEMPLATE = app
TARGET = syncer-init

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

LIBS += -lssh

unix:target.path = /usr/local/bin
macx:target.path = /Applications/syncer
unix|macx:INSTALLS += target

SOURCES += main.cpp\
    init.cpp

HEADERS  += \
    init.h

FORMS    += init.ui

OTHER_FILES +=

RESOURCES += 
