QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
QT       += core gui network
QT += testlib

QT_CONFIG_FLAGS += -openssl-linked

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuxPPClient
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    cryptor.cpp \
    user.cpp \
    client.cpp \
    tests.cpp \
    loginscreen.cpp \
    userlistmodel.cpp \
    aes.c \
    havege.c \
    sha512.c \
    timing.c


HEADERS  += mainwindow.h \
    cryptor.h \
    user.h \
    client.h \
    tests.h \
    loginscreen.h \
    userlistmodel.h \
    polarssl/aes.h \
    polarssl/check_config.h \
    polarssl/config.h \
    polarssl/havege.h \
    polarssl/sha512.h \
    polarssl/timing.h

FORMS    += mainwindow.ui \
    loginscreen.ui

LIBS += -L"C:\OpenSSL" -llibeay32
