#ifndef KRANICH_H
#define KRANICH_H

#pragma once

// STL
#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>

// QT
#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QFileDialog>
#include <QImage>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QProgressBar>

// CV
#include <opencv.hpp>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p != nullptr) { delete (p); (p) = nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p != nullptr) { delete[] (p); (p) = nullptr; } }
#endif

using namespace std;

#define KN_WIDGET_EMPTY 0
#define KN_WIDGET_LOAD	1
#define KN_WIDGET_MAIN	2

// PROJECT
#include "image.h"
#include "mainwindow.h"
//#include "dataset.h"

#endif
