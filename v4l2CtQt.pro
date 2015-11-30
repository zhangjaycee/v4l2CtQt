#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T11:25:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = v4l2CtQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    convert.cpp \
    v4l2grab.cpp \
    compressivetracker.cpp \
    handfinder.cpp

HEADERS  += mainwindow.h \
    v4l2grab.h \
    define.h \
    convert.h \
    compressivetracker.h \
    handfinder.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/arm/opencv/include\
/usr/local/arm/opencv/include/opencv\
/usr/local/arm/opencv/include/opencv2

LIBS += /usr/local/arm/opencv/lib/libopencv_highgui.so\
/usr/local/arm/opencv/lib/libopencv_core.so\
/usr/local/arm/opencv/lib/libopencv_imgproc.so
