TEMPLATE = app
<<<<<<< HEAD

CONFIG(debug, debug|release){
	message("Debug")
	TARGET = syncer-gui
=======
TARGET = syncer-gui

CONFIG(debug, debug|release){
	message("Debug")
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581
	DESTDIR = debug
        OBJECTS_DIR = debug
        MOC_DIR = debug
}else{
	message("Release")
<<<<<<< HEAD
	TARGET = syncer-gui
=======
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581
	DESTDIR = ../bin
	OBJECTS_DIR = release
	MOC_DIR = release
}

<<<<<<< HEAD
unix:target.path = /usr/bin
#target.path = $$[QT_INSTALL_BINS]
unix:INSTALLS += target

SOURCES += main.cpp\
    syncerlauncher.cpp

HEADERS  += \
    syncerlauncher.h

FORMS    +=
=======
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
>>>>>>> 56d4365c719c90d1f0874862f6f9c3ff69af3581

OTHER_FILES +=

RESOURCES += \
    icons.qrc
