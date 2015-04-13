#-------------------------------------------------
#
# Project created by QtCreator 2014-08-25T13:54:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vcounter
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    BlobTracking.cpp \
    VehicleCounting.cpp \
    PixelBased.cpp \
    PBAS.cpp \
    cvblob.cpp \
    cvcontour.cpp \
    cvtrack.cpp \
    cvlabel.cpp \
    cvaux.cpp \
    cvcolor.cpp \
    SJN_MultiCueBGS.cpp \
    MixtureOfGaussianV2BGS.cpp \
    sdLaMa091.cpp \
    SigmaDeltaBGS.cpp \
    SuBSENSE.cpp \
    BackgroundSubtractorSuBSENSE.cpp \
    BackgroundSubtractorLBSP.cpp \
    LBSP.cpp

HEADERS  += mainwindow.h \
    BlobTracking.h \
    VehicleCounting.h \
    PixelBased.h \
    PBAS.h \
    cvblob.h \
    quantile.h \
    SJN_MultiCueBGS.h \
    MixtureOfGaussianV2BGS.h \
    sdLaMa091.h \
    SigmaDeltaBGS.h \
    SuBSENSE.h \
    BackgroundSubtractorSuBSENSE.h \
    BackgroundSubtractorLBSP.h \
    LBSP.h \
    DistanceUtils.h \
    RandUtils.h

FORMS    += mainwindow.ui

INCLUDEPATH += "/usr/local/Cellar/opencv/2.4.9/include"
INCLUDEPATH += "/usr/local/Cellar/opencv/2.4.9/include/opencv"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_calib3d.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_contrib.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_core.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_features2d.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_flann.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_gpu.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_highgui.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_imgproc.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_legacy.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_ml.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_objdetect.dylib"
LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libopencv_video.dylib"
#LIBS += "/usr/local/Cellar/opencv/2.4.9/lib/libcvblob.dylib"

INCLUDEPATH += "/Users/yohanssen/Downloads/boost_1_54_0"

INCLUDEPATH += "/Applications/XAMPP/xamppfiles/include"
LIBS += "/Applications/XAMPP/xamppfiles/lib/libmysqlclient.18.dylib"

CONFIG += debug_and_release

OTHER_FILES += \
    LBSP_16bits_dbcross_1ch.i \
    LBSP_16bits_dbcross_3ch1t.i \
    LBSP_16bits_dbcross_3ch3t.i \
    LBSP_16bits_dbcross_s3ch.i


