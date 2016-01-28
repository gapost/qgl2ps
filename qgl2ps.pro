#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T20:46:24
#
#-------------------------------------------------

QT       += core gui opengl svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qgl2ps
TEMPLATE = app




SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

unix: LIBS += -lgl2ps -lglut

OTHER_FILES += \
    README.md

RESOURCES += \
    qgl2ps.qrc
