TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Matrix.cpp
QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS += -Wall

HEADERS += \
    NegativeIndexException.hpp \
    OutOfBoundsException.hpp \
    Matrix.hpp
