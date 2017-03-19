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
class QLineEdit;
class QGroupBox;

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
	std::vector<std::string>	m_filenames;

private slots:
	void importImages();

	void changeImage(int value);
	void changeRadius(int value);
	void changeThreshold(int value);

	void evaluate();
	void saveImage();

	cv::Mat		  drawImage();


private:
	int			m_index;
	int			m_halfwidth;
	int			m_halfheight;
	int			m_radius;
	int			m_threshold;

	cv::Mat		m_frame;

	QAction* m_action_import;

	QLabel*  m_label_frame;
	QLabel*	 m_label_filename;

	QPushButton* m_button_import;
	QPushButton* m_button_save;

	QProgressBar* m_progress_load;
	QSlider*	  m_slider_images;
	QSlider*	  m_slider_threshold;
	QSlider*	  m_slider_radius;

	QLabel*		  m_label_threshold;
	QLabel*		  m_label_radius;
	
	QCheckBox*	  m_check_show_radius;
	QCheckBox*	  m_check_show_links;
	QCheckBox*	  m_check_show_links_selected;

	QCheckBox*	  m_check_show_boundingbox;
	QCheckBox*	  m_check_show_sphere;
	QCheckBox*	  m_check_show_polygon;

	QCheckBox*	  m_check_show_boundingbox_next;
	QCheckBox*	  m_check_show_sphere_next;
	QCheckBox*	  m_check_show_polygon_next;

	QLineEdit*	  m_line_date;
	QLineEdit*	  m_line_abrasive_material;
	QLineEdit*	  m_line_particle_size_distribution;
	QLineEdit*	  m_line_blast_cabinet_pressure;
	QLineEdit*	  m_line_sample_identifier;
	QLineEdit*	  m_line_experiment_label;
	QLineEdit*	  m_line_revolutions_per_minute;
	QLineEdit*	  m_line_flow;
	QLineEdit*	  m_line_particle_flow;

	QLineEdit*	  m_line_scale_time;
	QLineEdit*	  m_line_scale_distance;
	QLineEdit*	  m_line_scale_density;

	QGroupBox*	  m_groupbox_frame;
	QGroupBox*	  m_groupbox_frame_next;

	QPushButton*  m_button_export;
	
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

	ParticlesInfo getParticles(const cv::Mat& in, int thresh = 100);
	double		  calcFriends(int a, int b);
};

#endif
