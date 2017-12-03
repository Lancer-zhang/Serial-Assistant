#-------------------------------------------------
#
# Project created by QtCreator 2017-08-06T22:45:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSerialAssistant
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    plotter.cpp \
    flappybrid/flappybrid.cpp \
    flappybrid/gameelement.cpp \
    flappybrid/background.cpp \
    flappybrid/ground.cpp \
    flappybrid/brid.cpp \
    flappybrid/title.cpp \
    flappybrid/readyboard.cpp \
    flappybrid/scoreboard.cpp \
    flappybrid/overboard.cpp \
    flappybrid/pipe.cpp \
    qnavigationwidget.cpp \
    screenshot/screenshot.cpp \
    screenshot/screencanvas.cpp

HEADERS += \
        mainwindow.h \
    structrue.h \
    constant.h \
    tool.h \
    plotter.h \
    flappybrid/flappybrid.h \
    flappybrid/gameelement.h \
    flappybrid/background.h \
    flappybrid/ground.h \
    flappybrid/brid.h \
    flappybrid/title.h \
    flappybrid/readyboard.h \
    flappybrid/scoreboard.h \
    flappybrid/overboard.h \
    flappybrid/pipe.h \
    qnavigationwidget.h \
    screenshot/screenshot.h \
    screenshot/screencanvas.h
QT += serialport
FORMS += \
        mainwindow.ui

RESOURCES += \
    rec.qrc

DISTFILES +=
