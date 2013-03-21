#-------------------------------------------------
#
# Project created by QtCreator 2013-02-21T13:01:12
#
#-------------------------------------------------

QT       += core console
QT		 += gui
#QT		 -= gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wumpus
TEMPLATE = app


SOURCES += \
#	src/multi_client/serveur/serveur.c \
#	src/multi_client/serveur/jeux.c \
#	src/multi_client/client/client2.c \
#			Client/client.cpp \
			main.cpp \
			UI/mainwindow.cpp \
			UI/pseudo.cpp \
			UI/option.cpp \
			Controleur/controleur.cpp \
			Client/client.c \



HEADERS  += \
#			src/multi_client/serveur/serveur.h \
#			src/multi_client/serveur/jeux.h \
			Client/client.h\
			UI/mainwindow.h \
			UI/pseudo.h \
			UI/option.h \
			Controleur/controleur.h \




FORMS    += UI/mainwindow.ui \
			UI/pseudo.ui \
			UI/option.ui \

RESOURCES += Resource.qrc
