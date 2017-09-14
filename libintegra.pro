#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T14:12:00
#
#-------------------------------------------------

QT       -= core gui

CONFIG += c++11

TARGET = integra
TEMPLATE = lib

DEFINES += LIBRARY_BUILD \
           UNITTEST
OBJECTS_DIR = obj
DESTDIR = ../lib
SOURCES += \
    ipaddress.cpp \
    jsonreport.cpp \
    stringutils.cpp \
    tcpsocket.cpp \
    netmask.cpp

HEADERS +=\
    ipaddress.h \
    jsonreport.h \
    stringutils.h \
    tcpsocket.h \
    def.h \
    netmask.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
