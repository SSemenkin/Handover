QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    handovermaker.cpp \
    handovers.cpp \
    handovers2g2g.cpp \
    handovers2g3g.cpp \
    main.cpp \
    mainwindow.cpp \
    network/QTelnet.cpp \
    network/telnet.cpp

HEADERS += \
    global.h \
    handovermaker.h \
    handovers.h \
    handovers2g2g.h \
    handovers2g3g.h \
    mainwindow.h \
    network/QTelnet.h \
    network/telnet.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    templates/2g2g/1.txt \
    templates/2g2g/2.txt \
    templates/2g2g/3.txt \
    templates/2g2g/4.txt \
    templates/2g2g/5.txt \
    templates/2g3g/1.txt \
    templates/2g3g/2.txt \
    templates/2g3g/3.txt \
    templates/2g3g/4.txt \
    templates/2g3g/5.txt
