QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuxPPClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversession.cpp \
    cryptor.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    serversession.h \
    cryptor.h \
    user.h

FORMS    += mainwindow.ui
