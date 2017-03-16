QT += core gui widgets

TARGET = kranich
TEMPLATE = app

QMAKE_CXXFLAGS += -fopenmp

SOURCES +=	main.cpp \
			kranich.cpp \
			mainwindow.cpp \
			image.cpp

HEADERS += 	kranich.h \
			mainwindow.h \
			image.h
				
LIBS += -lopencv_core \
		-lopencv_highgui \
		-lopencv_imgproc \
		-fopenmp 
		
INCLUDEPATH += 	/usr/include/opencv2/ \
