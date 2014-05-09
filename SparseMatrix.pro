#-------------------------------------------------
#
# Project created by QtCreator 2014-04-11T23:47:02
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SparseMatrix
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    SparseArray.cpp \
    SparseMatrix.cpp \
    SparseIterator.cpp \
    GradientMethod.cpp \
    ProfileMatrix.cpp \
    LevelMatrix.cpp

HEADERS += \
    SparseArray.h \
    SparseMatrix.h \
    SparseIterator.h \
    GradientMethod.h \
    ../CholeskyReversal.h \
    CholeskyReversal.h \
    Preconditions.h \
    ProfileMatrix.h \
    LevelMatrix.h \
    LevelArray.h
