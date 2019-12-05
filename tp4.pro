#-------------------------------------------------
#
# Project created by QtCreator 2019-11-08T15:03:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp4
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
        mainwindow.cpp

HEADERS  += mainwindow.h
LIBS += D:/opencv-build/openbuild/install/x86/mingw/bin/libopencv_core340.dll
LIBS += D:/opencv-build/openbuild/install/x86/mingw/bin/libopencv_highgui340.dll
LIBS += D:/opencv-build/openbuild/install/x86/mingw/bin/libopencv_imgcodecs340.dll
LIBS += D:/opencv-build/openbuild/install/x86/mingw/bin/libopencv_imgproc340.dll
LIBS += D:/opencv-build/openbuild/install/x86/mingw/bin/libopencv_features2d340.dll
LIBS += D:/opencv-build/openbuild/install/x86/mingw/bin/libopencv_calib3d340.dll
INCLUDEPATH += -I D:/opencv-build/openbuild/install/include
