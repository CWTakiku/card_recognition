#-------------------------------------------------
#
# Project created by QtCreator 2017-12-14T13:27:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lazychecking
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH+=H:\opencv\opencv\build\include
             H:\opencv\opencv\build\include\opencv
             H:\opencv\opencv\build\include\opencv2

CONFIG(debug,debug|release) {
LIBS += -LH:\opencv\opencv\build\x64\vc14\lib \
    -lopencv_world320d
} else {
LIBS += -LH:\opencv\opencv\build\x64\vc14\lib \
    -lopencv_world320
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    service.cpp \
    picture.cpp \
    dialog.cpp \
    handle.cpp \
    gradeshow.cpp

HEADERS += \
        mainwindow.h \
    service.h \
    picture.h \
    student.h \
    dialog.h \
    handle.h \
    standard.h \
    gradeshow.h

FORMS += \
        mainwindow.ui \
    dialog.ui \
    gradeshow.ui

RESOURCES += \
    src.qrc \
    srcp.qrc
