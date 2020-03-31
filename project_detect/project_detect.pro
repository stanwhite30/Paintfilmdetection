#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T06:40:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project_detct
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
        mainwindow.cpp \
    detect.cpp \
    convert.cpp


HEADERS  += mainwindow.h \
    detect.h \
    convert.h

FORMS    += mainwindow.ui

LIBS += -lwiringPi \
        /usr/lib/arm-linux-gnueabihf/libopencv_calib3d.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_contrib.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_core.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_features2d.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_flann.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_gpu.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_legacy.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_ml.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_objdetect.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_ocl.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_photo.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_stitching.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_superres.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_ts.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_video.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_videostab.so \


INCLUDEPATH += /usr/include/opencv \
                /usr/include
