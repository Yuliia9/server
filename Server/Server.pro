#-------------------------------------------------
#
# Project created by QtCreator 2014-07-05T19:01:39
#
#-------------------------------------------------

QT       += core websockets
QT       += core network

QT       -= gui

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h
