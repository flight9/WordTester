#-------------------------------------------------
#
# Project created by QtCreator 2011-09-05T20:56:40
#
#-------------------------------------------------

QT       += core gui

CONFIG += qaxcontainer

TARGET = WordTester
TEMPLATE = app

TRANSLATIONS = WordTester_zh_CN.ts

SOURCES += main.cpp\
        dialog.cpp \
    word.cpp \
    wbutton.cpp \
    mdialog.cpp \
    sdelegate.cpp \
    wtable.cpp \
    tdialog.cpp \
    WSheet.cpp \
    menu/XActionMenu.cpp \
    menu/QMenuPrivate.cpp \
    voice/XpTTSVoice.cpp

HEADERS  += dialog.h \
    word.h \
    wbutton.h \
    mdialog.h \
    sdelegate.h \
    wtable.h \
    tdialog.h \
    WSheet.h \
    menu/XActionMenu.h \
    voice/XpTTSVoice.h \
    main.h

FORMS    += dialog.ui \
    mdialog.ui \
    tdialog.ui

RESOURCES += \
    WordTester.qrc

RC_FILE += myapp.rc

OTHER_FILES += \
    myapp.rc
















