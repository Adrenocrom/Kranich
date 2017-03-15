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
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QProgressBar>
#include <QVBoxLayout>

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


QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format);
QImage mat_to_qimage_cpy(cv::Mat const &mat, QImage::Format format);
cv::Mat qimage_to_mat_ref(QImage &img, int format);
cv::Mat qimage_to_mat_cpy(QImage const &img, int format);

//extern RNG rng;

// PROJECT
#include "image.h"
#include "mainwindow.h"

#endif
