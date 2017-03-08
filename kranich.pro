QT += core gui widgets websockets opengl

TARGET = kranich
TEMPLATE = app

QMAKE_CXXFLAGS += -fopenmp

SOURCES +=	main.cpp \
			kranich.cpp \
			mainwindow.cpp \
			dataset.cpp

HEADERS += 	kranich.h \
			mainwindow.h \
			dataset.h
				
LIBS += -lopencv_core \
		-lopencv_highgui \
		-lopencv_imgproc \
		-fopenmp 
		
INCLUDEPATH += 	/usr/include/opencv2/