#-------------------------------------------------
#
# Project created by QtCreator 2013-02-21T13:01:12
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wumpus
TEMPLATE = app


SOURCES += src/multi_client/serveur/serveur.c
	#main.cpp \
	#UI/mainwindow.cpp \
	#src/multi_client/client/client.c \


HEADERS  += src/multi_client/serveur/serveur.h
	#UI/mainwindow.h \



FORMS    += UI/mainwindow.ui
