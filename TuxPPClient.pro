QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuxPPClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cryptor.cpp \
    user.cpp \
    session.cpp

HEADERS  += mainwindow.h \
    cryptor.h \
    user.h \
    session.h

FORMS    += mainwindow.ui
