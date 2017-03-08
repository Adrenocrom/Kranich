#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>

struct Dataset;

class QLabel;
class QPushButton;
class QProgressBar;
class QStackedWidget;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	Dataset* m_dataset;

private slots:
	void importImages();
	void exportDataset();

private:

	QAction* m_action_import;
	QAction* m_action_export;

	QLabel*  m_label_frame_first;
	QLabel*  m_label_frame_second;

	QPushButton* m_button_import;

	QProgressBar* m_progress_load;

	void createMenu();
	void createWidgets();
	void createWidgetEmpty();
	void createWidgetLoad();
	void createWidgetMain();

	QStackedWidget*	m_widget_stacked;
	QWidget*		m_widget_empty;
	QWidget*		m_widget_load;
	QWidget* 		m_widget_main;
};

#endif
