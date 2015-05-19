QT      += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = plot-examples
TEMPLATE = app


win32: LIBS += -L$$PWD/vrpn/ -lvrpn

INCLUDEPATH += $$PWD/vrpn/include
DEPENDPATH += $$PWD/vrpn/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/vrpn/vrpn.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/vrpn/libvrpn.a

SOURCES += main.cpp \
    mainwindow.cpp \
    qcustomplot/qcustomplot.cpp \
    vibe.cpp


CONFIG += console \
          c++11


HEADERS += \
    mainwindow.h \
    qcustomplot/qcustomplot.h \
    vibe.h

FORMS += \
    mainwindow.ui

