TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    vm.cpp \
    vasm.cpp \
    compiler.cpp

HEADERS += \
    grammar.h \
    grammar_def.h \
    compiler.h \
    vm.h \
    calculator.h \
    vasm.h \
    user_function.h \
    ast.h


INCLUDEPATH += E:/include/boost-1_59
LIBS += "-LE:/lib"

OTHER_FILES += \
    info_c_bnf.txt
