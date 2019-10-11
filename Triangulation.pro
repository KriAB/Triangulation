TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        delaunay.cpp \
        main.cpp \
        quadtree.cpp \
        regular.cpp \
        vector3d.cpp

HEADERS += \
    delaunay.h \
    lasloader.h \
    quadtree.h \
    regular.h \
    vector3d.h \
    sweepline.h 

