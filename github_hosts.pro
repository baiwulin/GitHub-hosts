#-------------------------------------------------
#
# Project created by QtCreator 2024-02-08T08:03:24
#
#-------------------------------------------------

QT       += core gui
QT +=network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = github_hosts
TEMPLATE = app
RC_ICONS += logo.ico
RC_FILE += mainrc.rc

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
    form_update.cpp

HEADERS += \
        mainwindow.h \
    form_update.h

FORMS += \
        mainwindow.ui \
    form_update.ui

RESOURCES += \
    icon.qrc

DISTFILES += \
    UAC.manifest \
    mainrc.rc


