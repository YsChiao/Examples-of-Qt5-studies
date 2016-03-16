#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T21:01:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = app


SOURCES += main.cpp\
        imageviewer.cpp

HEADERS  += imageviewer.h

FORMS    += imageviewer.ui



INCLUDEPATH += C:/opencv/opencv_debug/install/include
LIBS += -LC:/opencv/opencv_debug/install/x64/mingw/lib \
    -llibopencv_core2411d \
    -llibopencv_highgui2411d \
    -llibopencv_imgproc2411d

INCLUDEPATH += C:/opencv/opencv_release/install/include
LIBS += -LC:/opencv/opencv_release/install/x64/mingw/lib \
    -llibopencv_core2411 \
    -llibopencv_highgui2411 \
    -llibopencv_imgproc2411

RESOURCES += \
    resource.qrc
