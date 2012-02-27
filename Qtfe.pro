TEMPLATE = vclib

TARGET = Qtfe

INCLUDEPATH += include/

HEADERS += include/Qtfe.h
HEADERS += include/QtfeCanal.h
HEADERS += include/QtfeOutput.h

SOURCES += src/Qtfe.cpp
SOURCES += src/QtfeCanal.cpp 
SOURCES += src/QtfeOutput.cpp


CONFIG = qt debug warn_on staticlib
QT = core gui xml

MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = bin
