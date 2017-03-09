#include "kranich.h"


using namespace cv;

MainWindow::MainWindow() {
	// labels
	m_label_frame_first  = nullptr;
	m_label_frame_second = nullptr;

	// widgets
	m_widget_stacked 	= nullptr;
	m_widget_empty		= nullptr;

	createMenu();
	createWidgets();

	adjustSize();
}

MainWindow::~MainWindow() {
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
	
	m_label_frame_first = new QLabel("");
	m_label_frame_first->setMinimumSize(256, 256);
	m_label_frame_first->setMaximumSize(256, 256);

	m_label_frame_second = new QLabel("");
	m_label_frame_second->setMinimumSize(256, 256);
	m_label_frame_second->setMaximumSize(256, 256);

	m_slider_pairs = new QSlider(Qt::Horizontal);
	connect(m_slider_pairs, &QSlider::sliderMoved, this, &MainWindow::changeImage);

	gridLayout->addWidget(new QLineEdit(), 0, 0, 1, 2);
	gridLayout->addWidget(m_label_frame_first, 1, 0);
	gridLayout->addWidget(m_label_frame_second, 1, 1);
	gridLayout->addWidget(m_slider_pairs, 2, 0, 1, 2);
	m_widget_main->setLayout(gridLayout);
	m_widget_stacked->addWidget(m_widget_main);
}

void MainWindow::importImages() {
	QStringList filenames = QFileDialog::getOpenFileNames(this,tr("BMP files"),QDir::currentPath(),tr("Bitmap files (*.bmp);;All files (*.*)") );
	m_images.clear();

	if( !filenames.isEmpty() ) {
		m_widget_stacked->setCurrentIndex(KN_WIDGET_LOAD);
		this->repaint();	
		m_progress_load->setRange(0, filenames.count()-1);
		m_progress_load->setValue(0);

		filenames.sort();
    	
		for (int i = 0; i < filenames.count(); i++) {
			m_progress_load->setValue(i);
    	    cout<<filenames.at(i).toStdString()<<endl;
			QImage img(filenames.at(i));
			m_images.push_back(img);
		}

		CImage test(&m_images[0]);
		//test.globalThreshold(120);
		test.invert();
		test.regionGrowing(120, 4000, 0);

		m_label_frame_first->setPixmap(QPixmap::fromImage(m_images[0]).scaled(256, 256));
		m_label_frame_second->setPixmap(QPixmap::fromImage(test.get()).scaled(256, 256));

		m_slider_pairs->setMaximum(filenames.count()-1);

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

void MainWindow::changeImage(int value) {
	m_label_frame_second->setPixmap(QPixmap::fromImage(m_images[value]).scaled(256, 256));
}

// calculate the ssim
Scalar MainWindow::getMSSIM( const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d     = CV_32F;

    Mat I1, I2;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    Mat I2_2   = I2.mul(I2);        // I2^2
    Mat I1_2   = I1.mul(I1);        // I1^2
    Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    Mat mu1, mu2;   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2   =   mu1.mul(mu1);
    Mat mu2_2   =   mu2.mul(mu2);
    Mat mu1_mu2 =   mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    ///////////////////////////////// FORMULA ////////////////////////////////
    Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    Mat ssim_map;
    divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

    Scalar mssim = mean( ssim_map ); // mssim = average of ssim map

	mssim[3] = (mssim[0] + mssim[1] + mssim[2]) / 3.0; // save mead in alpha

return mssim;
}
