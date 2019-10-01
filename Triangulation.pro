TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        delaunay.cpp \
        filereadertxt.cpp \
        main.cpp \
        regular.cpp \
        vector3d.cpp

HEADERS += \
    delaunay.h \
    filereadertxt.h \
    regular.h \
    vector3d.h
