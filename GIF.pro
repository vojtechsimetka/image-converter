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

