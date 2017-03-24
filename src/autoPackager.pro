#-------------------------------------------------
#
# Project created by QtCreator 2017-03-06T12:34:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autoPackager
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


SOURCES += main.cpp\
    extractor.cpp \
    selectdir.cpp \
    maindialog.cpp \
    fileproject.cpp \
    settings.cpp \
    progressdialog.cpp

HEADERS  += \
    extractor.h \
    selectdir.h \
    maindialog.h \
    fileproject.h \
    registries.h \
    settings.h \
    progressdialog.h

FORMS    += \
    selectdir.ui \
    maindialog.ui \
    progressdialog.ui

RESOURCES += \
    iconresource.qrc

#ico
RC_FILE+="icons.rc"

#implement of quaZip and zlib from my dir
win32 {
INCLUDEPATH += "$$PWD/QtLibraries/zlib-1.2.11-win32"
LIBS += -L "$$PWD/QtLibraries/zlib-1.2.11-win32" -lz

INCLUDEPATH += "$$PWD/QtLibraries/quazip-0.7.3-win32/quazip"
LIBS += -L$$PWD/QtLibraries/quazip0.7.3-win32/release -lquazip
}
