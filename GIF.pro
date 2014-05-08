QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    arguments.cpp \
    imageprocessing.cpp \
    bmp.cpp \
    dictionary.cpp \
    gif.cpp \
    gif2bmp.cpp \
    gifencoder.cpp \
    gifwriter.cpp

HEADERS += \
    arguments.h \
    imageprocessing.h \
    bmp.h \
    constant.h \
    dictionary.h \
    gif.h \
    gif2bmp.h \
    gifencoder.h \
    subblock.h \
    gifwriter.h \
    gifdictionary.h

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_objdetect \
    -lopencv_calib3d

INCLUDEPATH += /usr/local/include/opencv
DEPENDPATH += /usr/local/include/opencv/include

INCLUDEPATH += $$PWD/../../../usr/local/include
DEPENDPATH += $$PWD/../../../usr/local/include
DEPENDPATH += $$PWD/../../../usr/local/lib
