TARGET = greeting

QT += network gui
#QT -= gui

INCLUDEPATH += ../greeting/lib
LIBS += -L../greeting/lib

LIBS += -lqhttpserver

SOURCES = greeting.cpp \
    singleapplication.cpp
HEADERS = greeting.h \
    singleapplication.h
