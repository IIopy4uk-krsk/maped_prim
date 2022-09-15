#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T17:11:54
#
#-------------------------------------------------

QT       += core gui widgets concurrent

TARGET = maped
TEMPLATE = app

CONFIG += debug c++17

SOURCES += main.cpp\
    mapfile.cpp \
    mainwindow.cpp \
    custom.cpp

HEADERS  += \
    custom.h \
    mapfile.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui

RESOURCES +=


QMAKE_LFLAGS += -static

QMAKE_CXXFLAGS += -Wall -pedantic -Wextra -Werror -Wcast-align -Wcast-qual
QMAKE_CXXFLAGS += -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs
QMAKE_CXXFLAGS +=  -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
QMAKE_CXXFLAGS += -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused
QMAKE_CXXFLAGS += -Wno-variadic-macros -Wno-parentheses-fdiagnostics-show-option

# ругается на Qt
#QMAKE_CXXFLAGS += -Wshadow -Wctor-dtor-privacy -Wnoexcept -Wsign-promo

