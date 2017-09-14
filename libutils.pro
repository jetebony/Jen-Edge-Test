#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T14:12:00
#
#-------------------------------------------------

QT       -= core gui

CONFIG += c++11
CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

TARGET = integra
TEMPLATE = lib

DEFINES += LIBRARY_BUILD UNITTEST

OBJECTS_DIR = obj
DESTDIR = ../lib
SOURCES += \
    jsonreport.cpp \
    stringutils.cpp \
    main.cpp \
    jsonreport_ut.cpp \
    stringutils_ut.cpp \
    logger.cpp \
    mossie.cpp \
    mossie_ut.cpp \
    logger_ut.cpp \
    datetimeutils.cpp \
    datetimeutils_ut.cpp

INCLUDEPATH += ..

HEADERS +=\
    jsonreport.h \
    stringutils.h \
    def.h \
LIBS += -L/usr/lib/x86_64-linux-gnu/ -ljson-c \
    logger.h \
    mossie.h \
    datetimeutils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    Makefile \
    README.md
