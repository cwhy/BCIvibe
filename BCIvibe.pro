QT      += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = plot-examples
TEMPLATE = app

SOURCES += main.cpp \
    qcustomplot/qcustomplot.cpp \
    vibe.cpp \
    signalplot.cpp \
    thermometer.cpp \
    metricplot.cpp \
    signalplotwindow.cpp \
    metricplotwindow.cpp

INCLUDEPATH += $$PWD/vrpn/include
DEPENDPATH += $$PWD/vrpn/include

CONFIG += console \
          c++11


HEADERS += \
    qcustomplot/qcustomplot.h \
    vibe.h \
    signalplot.h \
    thermometer.h \
    channellocation.h \
    metricplot.h \
    signalplotwindow.h \
    metricplotwindow.h \
    ui_metricplot.h \
    ui_signalplot.h

#FORMS += \
#    mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/vrpn/ -lvrpn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/vrpn/ -lvrpnd
