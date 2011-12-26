TEMPLATE = app

release {
        TARGET = ../../bin/restore
        OBJECTS_DIR = release
        MOC_DIR = release
}
debug {
        TARGET = restore
        OBJECTS_DIR = debug
        MOC_DIR = debug
}


DEPENDPATH += . 
INCLUDEPATH += . 

# Input
HEADERS += restore.h
FORMS += restore.ui
SOURCES += main.cpp restore.cpp
