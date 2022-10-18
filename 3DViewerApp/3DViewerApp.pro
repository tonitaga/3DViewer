QT       += core gui
include(./QtGifImage-master/src/gifimage/qtgifimage.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../src/s21_3DViewer.c \
    ../src/s21_MoveModel.c \
    main.cpp \
    mainwindow.cpp \
    settingsmodel.cpp \
    viewmodel.cpp \
    eastereggs.cpp

HEADERS += \
    ../src/s21_3dViewer.h \
    mainwindow.h \
    settingsmodel.h \
    viewmodel.h \
    eastereggs.h

FORMS += \
    mainwindow.ui \
    settingsmodel.ui \
    eastereggs.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    pictures.qrc
