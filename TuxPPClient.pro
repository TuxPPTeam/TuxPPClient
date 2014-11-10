QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuxPPClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cryptor.cpp \
    user.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    cryptor.h \
    user.h \
    client.h

FORMS    += mainwindow.ui
