#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T19:52:38
#
#-------------------------------------------------

QT       += core gui \
        multimedia \
        multimediawidgets

QT       += sql
QT += network
QT       += core gui webenginewidgets

#QT += webkit

#QT+= webkitwidgets
#QT += webkit network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MediaPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mediaplayer.cpp \
    customslider.cpp \
    mainwidget.cpp \
    musicplayer.cpp \
    sql.cpp \
    usersql.cpp \
    start.cpp

HEADERS  += mediaplayer.h \
    customslider.h \
    mainwidget.h \
    musicplayer.h \
    sql.h \
    usersql.h \
    start.h

FORMS    += mediaplayer.ui \
    mainwidget.ui \
    musicplayer.ui \
    sql.ui \
    usersql.ui \
    start.ui

RESOURCES += \
    Photo.qrc
