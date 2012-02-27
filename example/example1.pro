TEMPLATE = vcapp
TARGET   = example1

SOURCES  += example1.cpp

LIBS *= Qtfe.lib

CONFIG = qt release warn_on
QT = core gui xml

MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = bin
