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

unix:target.path = /usr/bin
#target.path = $$[QT_INSTALL_BINS]
unix:INSTALLS += target

SOURCES += main.cpp\
    syncerlauncher.cpp

HEADERS  += \
    syncerlauncher.h

FORMS    +=

OTHER_FILES +=

RESOURCES += \
    icons.qrc
