TEMPLATE = app


release {
        TARGET = ../../bin/syncer-gui
        OBJECTS_DIR = release
        MOC_DIR = release
}
debug {
        TARGET = syncer-gui
        OBJECTS_DIR = debug
        MOC_DIR = debug
}

SOURCES += main.cpp\
    syncerlauncher.cpp

HEADERS  += \
    syncerlauncher.h

FORMS    +=

OTHER_FILES +=

RESOURCES += \
    icons.qrc
