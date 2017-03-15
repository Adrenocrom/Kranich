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
class QCheckBox;

struct ParticlesInfo {
	std::vector<std::vector<cv::Point> > contours_poly;
	std::vector<cv::Rect> 		   	boundRect;
	std::vector<cv::Point2f>  		center;
	std::vector<float>				radius;
	std::vector<std::vector<std::pair<int, double> > >  friends;
};

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	std::vector<QImage> 		m_images;
	std::vector<cv::Mat> 		m_cv_images;
	std::vector<ParticlesInfo> 	m_particleinfos;


private slots:
	void importImages();
	void exportDataset();

	void changeImage(int value);
	void changeRadius(int value);

	void createPairs();

	cv::Mat		  drawImage();


private:
	int			m_index;
	int			m_halfwidth;
	int			m_halfheight;
	int			m_radius;

	QAction* m_action_import;
	QAction* m_action_export;

	QLabel*  m_label_frame;

	QPushButton* m_button_import;

	QProgressBar* m_progress_load;
	QSlider*	  m_slider_pairs;
	QSlider*	  m_slider_radius;

	QCheckBox*	  m_check_show_boundingbox;
	QCheckBox*	  m_check_show_sphere;
	QCheckBox*	  m_check_show_polygon;
	QCheckBox*	  m_check_show_radius;

	QCheckBox*	  m_check_show_boundingbox_next;
	QCheckBox*	  m_check_show_sphere_next;
	QCheckBox*	  m_check_show_polygon_next;
	
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
	double getPSNR(const cv::Mat& I1, const cv::Mat& I2);

	ParticlesInfo getParticles(const cv::Mat& in);
	bool		  calcFriends(int a, int b);
};

#endif
