#-------------------------------------------------
#
# Project created by QtCreator 2015-03-13T00:37:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yt-downloader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ytdownload.cpp \
    url.cpp \
    ytsrc_definitions.cpp \
    ytsrc_parser.cpp \
    ytsrc_scanner.cpp \
    progressthread.cpp \
    threaddl.cpp

HEADERS  += mainwindow.h \
    ytdownload.h \
    url.hpp \
    ytsrc_definitions.hpp \
    ytsrc_parser.hpp \
    progressthread.h \
    threaddl.h

FORMS    += mainwindow.ui

unix:!macx|win32: LIBS += -LC:/Toolkits/curl32/lib/ -lcurldll

INCLUDEPATH += C:/Toolkits/curl32/include
DEPENDPATH += C:/Toolkits/curl32/include
INCLUDEPATH += C:\Toolkits\utf8_v2_3_4\source
