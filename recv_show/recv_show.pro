QT       += core gui
QT += network
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    audioaacplayer.cpp \
    audiotcpserver.cpp \
    audiofaaddecoder.cpp \
    audioaacfifobuf.cpp \
    videoh264tomat.cpp \
    videodisplaywidget.cpp \
    videoframedrawthread.cpp \
    videotcpserver.cpp \
    videomatqueue.cpp \
    widgetbody.cpp \
    widgettitlebar.cpp \
    widgetvideobtn.cpp


HEADERS += \
    mainwindow.h \
    playmodule.h \
    audioaacplayer.h \
    audiotcpserver.h \
    audiofaaddecoder.h \
    videoh264tomat.h \
    videodisplaywidget.h \
    videoframedrawthread.h \
    videotcpserver.h \
    videomatqueue.h \
    widgetbody.h \
    widgettitlebar.h \
    widgetvideobtn.h \
    audioaacfifobuf.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

    INCLUDEPATH += C:\third\opencv_420\mingw_32bit_debug\install\include \
               C:\third\opencv_420\mingw_32bit_debug\install\include\opencv2 \
               C:\third\ffmpeg\20200424-a501947-win32-dev\include \
               C:\third\faad\include

    LIBS += C:\third\opencv_420\mingw_32bit_debug\install\x64\mingw\lib\libopencv_*  \
        C:\third\ffmpeg\20200424-a501947-win32-dev\lib\lib* \
        C:\third\faad\lib\lib*

#    INCLUDEPATH += C:\third\opencv_420\mingw_32bit_release\install\include \
#               C:\third\opencv_420\mingw_32bit_release\install\include\opencv2 \
#               C:\third\ffmpeg\20200424-a501947-win32-dev\include \
#               C:\third\faad\include

#    LIBS += C:\third\opencv_420\mingw_32bit_release\install\x64\mingw\lib\libopencv_*  \
#        C:\third\ffmpeg\20200424-a501947-win32-dev\lib\lib* \
#        C:\third\faad\lib\lib*


DISTFILES +=
