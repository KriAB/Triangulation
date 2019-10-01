TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        delaunay.cpp \
        main.cpp \
        regular.cpp

HEADERS += \
    delaunay.h \
    regular.h \
    sweepline.h
