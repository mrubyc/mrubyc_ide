#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T14:59:22
#
#-------------------------------------------------

QT       += core gui serialport network widgets xml
include(./highlighter/highlighter.pri)
include(./idesetting/idesetting.pri)
include(./project/project.pri)
include(./build/build.pri)

TARGET = mrubyc_ide
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mrctexteditor.cpp \
    linenumberarea.cpp \
    waitingspinner.cpp

HEADERS  += mainwindow.h \
    mrctexteditor.h \
    linenumberarea.h \
    waitingspinner.h \
    mrubyc_ide.h

FORMS    += mainwindow.ui \

RESOURCES += \
    icons.qrc

DISTFILES += \
    config/mrubyc_ide.ini

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

defineReplace(makeCopyCommand){
    DEPEND_FILE = $$1
    win32:{
        CONFIG(debug,debug|release){
            APP_BUILD_DIR=$$shadowed($${PWD})/debug
        }else{
            APP_BUILD_DIR=$$shadowed($${PWD})/release
        }
        DEPEND_FILE ~= s|/|\|gi
        APP_BUILD_DIR ~= s|/|\|gi
    }else{
        APP_BUILD_DIR=$$shadowed($${PWD})
    }
    return($(COPY_DIR) $$DEPEND_FILE $$APP_BUILD_DIR)
}
copyfile1.commands += $$makeCopyCommand($${PWD}/config/mruby_ide.ini)
QMAKE_EXTRA_TARGETS += copyfile1
