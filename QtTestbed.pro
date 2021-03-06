QT += gui widgets quick quickwidgets

TEMPLATE = lib
DEFINES += QTTESTBED_LIBRARY

CONFIG += c++11

include(../config.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += ANYRPC_DLL

SOURCES += \
    keyboarddevice.cpp \
    mousedevice.cpp \
    qobjecthelper.cpp \
    qobjectproxy.cpp \
    qtestbed.cpp \
    watcher.cpp

HEADERS += \
    QtTestbed_global.h \
    keyboarddevice.h \
    mousedevice.h \
    qobjecthelper.h \
    qobjectproxy.h \
    qtestbed.h \
    rpcfunctions.h \
    watcher.h

prod_test {
    SOURCES += rpcfunctions.cpp
    HEADERS += rpcfunctions.h
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

prod_test {

    win32: LIBS += -L$$PWD/../thirdparty/anyrpc/lib/ -lanyrpc

    INCLUDEPATH += $$PWD/../thirdparty/anyrpc/include
    DEPENDPATH += $$PWD/../thirdparty/anyrpc/include
}
