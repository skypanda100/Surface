#-------------------------------------------------
#
# Project created by QtCreator 2019-11-06T15:00:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets datavisualization

TARGET = Surface
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        dem.cpp \
        graph.cpp \
        inputwidget.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        dem.h \
        graph.h \
        inputwidget.h \
        mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 地形
map_file = $$PWD\\map.tif
CONFIG(debug, debug|release) {
    dst_dir = $$OUT_PWD\\debug\\
} else {
    dst_dir = $$OUT_PWD\\release\\
}
map_file ~= s,/,\\,g
dst_dir ~= s,/,\\,g
system(mkdir $$dst_dir)
system(copy $$map_file $$dst_dir /Y)

LIBS += -L'C:/Program Files/gdal/lib/' -lgdal_i
INCLUDEPATH += 'C:/Program Files/gdal/include'
DEPENDPATH += 'C:/Program Files/gdal/include'
PRE_TARGETDEPS += 'C:/Program Files/gdal/lib/gdal_i.lib'

RESOURCES += \
    resource.qrc

