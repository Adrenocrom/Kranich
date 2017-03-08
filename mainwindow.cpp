#include "kranich.h"

MainWindow::MainWindow() {
	// create dataset container
	m_dataset = nullptr;
	m_dataset = new Dataset;

	// labels
	m_label_frame_first  = nullptr;
	m_label_frame_second = nullptr;

	// widgets
	m_widget_stacked 	= nullptr;
	m_widget_empty		= nullptr;

	createMenu();
	createWidgets();
}

MainWindow::~MainWindow() {
	SAFE_DELETE(m_dataset);
	SAFE_DELETE(m_label_frame_first);
	SAFE_DELETE(m_label_frame_second);
	SAFE_DELETE(m_widget_empty);
	SAFE_DELETE(m_widget_stacked);
}

void MainWindow::createMenu() {
	QMenu* menu_file = menuBar()->addMenu(tr("&File"));
	
	m_action_import = new QAction(tr("import"), this);
	menu_file->addAction(m_action_import);
	m_action_export = new QAction(tr("export"), this);
	menu_file->addAction(m_action_export);
	menu_file->addSeparator();

	connect(m_action_import, &QAction::triggered, this, &MainWindow::importImages);
	connect(m_action_export, &QAction::triggered, this, &MainWindow::exportDataset);

	QAction* action_quit = menu_file->addAction(tr("&Quit"), this, &QWidget::close);
   	action_quit->setShortcuts(QKeySequence::Quit);
	action_quit->setStatusTip(tr("Quit the application"));
}

void MainWindow::createWidgets() {
	m_widget_stacked = new QStackedWidget;

	createWidgetEmpty();
	createWidgetLoad();
	createWidgetMain();

	setCentralWidget(m_widget_stacked);
}

void MainWindow::createWidgetEmpty() {
	m_widget_empty	  = new QWidget;
	
	QGridLayout* gridLayout = new QGridLayout;
	QGroupBox*	 groupBox	= new QGroupBox;
	QFormLayout* formLayout = new QFormLayout;

	m_button_import = new QPushButton("import images");
	connect(m_button_import, &QPushButton::pressed, this, &MainWindow::importImages);

	formLayout->addRow(new QLabel(""), m_button_import);

	groupBox->setMinimumWidth(300);
	groupBox->setLayout(formLayout);
	gridLayout->addWidget(groupBox, 0, 0, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
	
	m_widget_empty->setLayout(gridLayout);
	m_widget_stacked->addWidget(m_widget_empty);
}

void MainWindow::createWidgetLoad() {
	m_widget_load			= new QWidget;
	QGridLayout* gridLayout = new QGridLayout;

	m_progress_load 		= new QProgressBar;
	m_progress_load->setMinimumWidth(300);
	m_progress_load->setRange(0, 100);
	m_progress_load->setValue(0);
	gridLayout->addWidget(m_progress_load, 0, 0, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
	
	m_widget_load->setLayout(gridLayout);
	m_widget_stacked->addWidget(m_widget_load);
}

void MainWindow::createWidgetMain() {
	m_widget_main = new QWidget;
	QGridLayout* gridLayout = new QGridLayout;
	QGroupBox* 	 groupBox 	= new QGroupBox;
	QFormLayout* formLayout = new QFormLayout;

	formLayout->addRow(new QLabel("hier kommen die bilder hin"));

	groupBox->setLayout(formLayout);
	gridLayout->addWidget(groupBox, 0, 0, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
	m_widget_main->setLayout(gridLayout);
	m_widget_stacked->addWidget(m_widget_main);
}

void MainWindow::importImages() {
	QStringList filenames = QFileDialog::getOpenFileNames(this,tr("BMP files"),QDir::currentPath(),tr("Bitmap files (*.bmp);;All files (*.*)") );
	
	if( !filenames.isEmpty() && m_dataset != nullptr) {
		m_widget_stacked->setCurrentIndex(KN_WIDGET_LOAD);
		this->repaint();	
		m_progress_load->setRange(0, filenames.count()-1);
		m_progress_load->setValue(0);

		filenames.sort();
    	
		for (int i = 0; i < filenames.count(); i++) {
			m_progress_load->setValue(i);
    	    cout<<filenames.at(i).toStdString()<<endl;
			QImage img(filenames.at(i));
			m_dataset->images.push_back(img);
		}

		m_widget_stacked->setCurrentIndex(KN_WIDGET_MAIN);
		this->repaint();
	} else {
		m_widget_stacked->setCurrentIndex(KN_WIDGET_EMPTY);
		this->repaint();
	}
}

void MainWindow::exportDataset() {
	QString filename = QFileDialog::getSaveFileName(this,tr("CSV files"),QDir::currentPath(),tr("CSV files (*.csv);;All files (*.*)") );
}
