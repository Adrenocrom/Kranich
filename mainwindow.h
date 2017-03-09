#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>

#include <opencv.hpp>

#include "image.h"

class QLabel;
class QSlider;
class QPushButton;
class QProgressBar;
class QStackedWidget;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	std::vector<QImage> m_images;
	std::vector<CImage*> m_cimages;

private slots:
	void importImages();
	void exportDataset();

	void changeImage(int value);

private:

	QAction* m_action_import;
	QAction* m_action_export;

	QLabel*  m_label_frame_first;
	QLabel*  m_label_frame_second;

	QPushButton* m_button_import;

	QProgressBar* m_progress_load;
	QSlider*	  m_slider_pairs;

	void createMenu();
	void createWidgets();
	void createWidgetEmpty();
	void createWidgetLoad();
	void createWidgetMain();

	QStackedWidget*	m_widget_stacked;
	QWidget*		m_widget_empty;
	QWidget*		m_widget_load;
	QWidget* 		m_widget_main;

	cv::Scalar getMSSIM( const cv::Mat& i1, const cv::Mat& i2);
};

#endif
