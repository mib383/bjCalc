TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    vm.cpp \
    vasm.cpp \
    compiler.cpp \
    calculator.cpp

HEADERS += \
    grammar.h \
    grammar_def.h \
    compiler.h \
    vm.h \
    calculator.h \
    vasm.h \
    user_function.h \
    ast.h


INCLUDEPATH += D:/boost/1_60_0/include/boost-1_60
LIBS += "-Ld:/boost/1_60_0/lib"

OTHER_FILES +=

DISTFILES += \
    README.md


Release:DESTDIR = $$PWD/bin
Release:OBJECTS_DIR = $$PWD/out/release/.obj
Release:MOC_DIR = $$PWD/out/release/.moc
Release:RCC_DIR = $$PWD/out/release/.rcc
Release:UI_DIR = $$PWD/out/release/.ui

Debug:DESTDIR = $$PWD/out/debug
Debug:OBJECTS_DIR = $$PWD/out/debug/.obj
Debug:MOC_DIR = $$PWD/out/debug/.moc
Debug:RCC_DIR = $$PWD/out/debug/.rcc
Debug:UI_DIR = $$PWD/out/debug/.ui

