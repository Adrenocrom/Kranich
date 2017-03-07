#ifndef KRANICH_H
#define KRANICH_H

#pragma once

// STL
#include <stdio.h>
#include <iostream>

// QT
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
//#include <QString>
//#include <QStringList>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p != nullptr) { delete (p); (p) = nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p != nullptr) { delete[] (p); (p) = nullptr; } }
#endif

using namespace std;

// PROJECT
#include "mainwindow.h"

#endif
