QT += core gui widgets websockets opengl

TARGET = kranich
TEMPLATE = app

QMAKE_CXXFLAGS += -fopenmp

SOURCES +=	main.cpp \
			kranich.cpp \
			mainwindow.cpp

HEADERS += 	kranich.h \
			mainwindow.h
				
LIBS += -lboost_system \
		-lboost_filesystem \
		-lopencv_core \
		-lopencv_highgui \
		-lopencv_imgproc \
		-fopenmp 
		
INCLUDEPATH += 	/usr/include/opencv2/ \
				/usr/include/boost/
