TEMPLATE = vcapp
TARGET   = example2

HEADERS  += QImageTweak.h
SOURCES  += example2.cpp

LIBS *= Qtfe.lib

CONFIG = qt debug warn_on
QT = core gui xml

MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = bin
