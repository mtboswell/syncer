
TEMPLATE = app

release{
        message("Release")
        TARGET = syncer
        DESTDIR = ../bin
        OBJECTS_DIR = release
        MOC_DIR = release
}

debug{
        message("Debug")
        TARGET = syncer
        OBJECTS_DIR = debug
        MOC_DIR = debug
}

target.path = $$[QT_INSTALL_BINS]
INSTALLS += target

DEPENDPATH += .
INCLUDEPATH += . 

# Input
HEADERS += gitlauncher.h
SOURCES += gitlauncher.cpp main.cpp
