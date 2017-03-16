#include "kranich.h"


using namespace cv;


RNG rng(12345);

MainWindow::MainWindow() {
	// labels
	m_label_frame  = nullptr;

	// widgets
	m_widget_stacked 	= nullptr;
	m_widget_empty		= nullptr;

	m_radius = 50;

	createMenu();
	createWidgets();

	setMinimumSize(1024, 768);
	setMaximumSize(1024, 768);

}

MainWindow::~MainWindow() {
	SAFE_DELETE(m_label_frame);
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
	
	m_label_frame = new QLabel("");
	m_label_frame->setMinimumSize(256, 256);
	m_label_frame->setMaximumSize(256, 256);

	m_slider_images = new QSlider(Qt::Horizontal);
	connect(m_slider_images, &QSlider::valueChanged, this, &MainWindow::changeImage);

	m_slider_threshold = new QSlider(Qt::Horizontal);
	m_slider_threshold->setRange(0, 255);
	m_slider_threshold->setValue(100);
	connect(m_slider_threshold, &QSlider::valueChanged, this, &MainWindow::changeThreshold);
	m_label_threshold = new QLabel(tr("100"));

	m_slider_radius = new QSlider(Qt::Horizontal);
	m_slider_radius->setRange(0, 100);
	m_label_radius = new QLabel(tr("50"));
	connect(m_slider_radius, &QSlider::valueChanged, this, &MainWindow::changeRadius);

	gridLayout->addWidget(m_label_frame, 1, 0, 1, 6);
	gridLayout->addWidget(m_slider_images, 3, 0, 1, 6);
	gridLayout->addWidget(new QLabel("threshold"), 4, 0, 1, 1);
	gridLayout->addWidget(m_slider_threshold, 4, 1, 1, 4);
	gridLayout->addWidget(m_label_threshold, 4, 5, 1, 1);
	gridLayout->addWidget(new QLabel("radius"), 5, 0, 1, 1);
	gridLayout->addWidget(m_slider_radius, 5, 1, 1, 4);
	gridLayout->addWidget(m_label_radius, 5, 5, 1, 1);

	QGroupBox *groupBox = new QGroupBox(tr("this frame"));


	m_check_show_radius			= new QCheckBox(tr("show radius"));
	m_check_show_boundingbox 	= new QCheckBox(tr("show bounding box"));
	m_check_show_sphere 		= new QCheckBox(tr("show sphere"));
	m_check_show_polygon 		= new QCheckBox(tr("show polygon"));
	
	m_check_show_radius->setChecked(true);
	m_check_show_boundingbox->setChecked(true);
	m_check_show_sphere->setChecked(true);
	m_check_show_polygon->setChecked(true);

	connect(m_check_show_radius,	  &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	connect(m_check_show_boundingbox, &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	connect(m_check_show_sphere, 	  &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	connect(m_check_show_polygon, 	  &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_check_show_radius);
  	vbox->addWidget(m_check_show_boundingbox);
  	vbox->addWidget(m_check_show_sphere);
  	vbox->addWidget(m_check_show_polygon);
	
	//vbox->addStretch(1);
	groupBox->setLayout(vbox);
	gridLayout->addWidget(groupBox, 6, 0, 1, 3);


	QGroupBox *groupBoxN = new QGroupBox(tr("next frame"));

	m_check_show_boundingbox_next 	= new QCheckBox(tr("show bounding box"));
	m_check_show_sphere_next 		= new QCheckBox(tr("show sphere"));
	m_check_show_polygon_next 		= new QCheckBox(tr("show polygon"));

	m_check_show_boundingbox_next->setChecked(true);
	m_check_show_sphere_next->setChecked(true);
	m_check_show_polygon_next->setChecked(true);
	
	connect(m_check_show_boundingbox_next, 	&QCheckBox::stateChanged, this, &MainWindow::drawImage);
	connect(m_check_show_sphere_next, 	  	&QCheckBox::stateChanged, this, &MainWindow::drawImage);
	connect(m_check_show_polygon_next, 	  	&QCheckBox::stateChanged, this, &MainWindow::drawImage);

	QVBoxLayout *vboxN = new QVBoxLayout;
	vboxN->addWidget(m_check_show_boundingbox_next);
	vboxN->addWidget(m_check_show_sphere_next);
	vboxN->addWidget(m_check_show_polygon_next);
	//vboxN->addStretch(1);
	groupBoxN->setLayout(vboxN);
	gridLayout->addWidget(groupBoxN, 6, 3, 1, 3 );



	m_widget_main->setLayout(gridLayout);
	m_widget_stacked->addWidget(m_widget_main);
}

void MainWindow::importImages() {
	QStringList filenames = QFileDialog::getOpenFileNames(this,tr("BMP files"),QDir::currentPath(),tr("Bitmap files (*.bmp);;All files (*.*)") );
	m_images.clear();
	m_cv_images.clear();
	m_particleinfos.clear();
	if( !filenames.isEmpty() ) {
		m_widget_stacked->setCurrentIndex(KN_WIDGET_LOAD);
		this->repaint();	
		m_progress_load->setRange(0, filenames.count()-1);
		m_progress_load->setValue(0);

		filenames.sort();
	
		m_images.resize(filenames.count());
		m_cv_images.resize(filenames.count());
		m_particleinfos.resize(filenames.count());
		for(int i = 0; i < filenames.count(); i++) {
			m_progress_load->setValue(i);
			
			Mat src = imread(filenames.at(i).toStdString(), 1);

			cvtColor( src, m_cv_images[i], CV_BGR2GRAY );
   			blur( m_cv_images[i], m_cv_images[i], Size(3,3) );

			m_images[i]	= mat_to_qimage_cpy(src, QImage::Format_RGB888);
			m_particleinfos[i] = getParticles(m_cv_images[i]);
		}
		

		m_halfwidth  = (int)((float)m_cv_images[0].cols / 2.0f);
		m_halfheight = (int)((float)m_cv_images[0].rows / 2.0f);
			
		m_slider_images->setMaximum(filenames.count()-1);
		m_slider_images->setValue(0);
		m_index = 0;
		m_slider_radius->setValue(m_radius);


		m_label_frame->setMinimumSize(m_halfwidth, m_halfheight);
		m_label_frame->setMaximumSize(m_halfwidth, m_halfheight);
		drawImage();
		//m_label_frame->setPixmap(QPixmap::fromImage(drawImage()).scaled(m_halfwidth, m_halfheight));
			
	
		m_widget_stacked->setCurrentIndex(KN_WIDGET_MAIN);
		this->repaint();

		setMinimumSize(minimumSizeHint());
		setMaximumSize(minimumSizeHint());
		this->resize(minimumSizeHint());
	} else {
		m_widget_stacked->setCurrentIndex(KN_WIDGET_EMPTY);
		this->repaint();
	}
}

void MainWindow::changeThreshold(int value) {
	m_label_threshold->setText(QString::number(value));
	
	for(int i = 0; i < m_cv_images.size(); ++i) {
		m_particleinfos[i] = getParticles(m_cv_images[i], value);
	}

	drawImage();
}

void MainWindow::changeRadius(int value) {
	m_radius = value;
	m_label_radius->setText(QString::number(value));
	drawImage();
}

void MainWindow::exportDataset() {
	QString filename = QFileDialog::getSaveFileName(this,tr("CSV files"),QDir::currentPath(),tr("CSV files (*.csv);;All files (*.*)") );
}

void MainWindow::changeImage(int value) {
	m_index	= value;
	drawImage();
}

void MainWindow::createPairs() {
	
}

void MainWindow::evaluate() {
}
 
double MainWindow::calcFriends(int a, int b) {
	ParticlesInfo& pinfo 		= m_particleinfos[a];
	ParticlesInfo& pinfo_next 	= m_particleinfos[b];
	pinfo.friends.resize(pinfo.radius.size());

	for(int i = 1; i < pinfo.radius.size(); ++i) {
		list<pair<int, double> > friends_list;

		for(int j = 1; j < pinfo_next.radius.size(); ++j) {
			Point2f p = pinfo.center[i] - pinfo_next.center[j];

			if(norm(p) < (double)m_radius) {
				Mat I1 = m_cv_images[a](pinfo.boundRect[i]);
				Mat I  = m_cv_images[b](pinfo_next.boundRect[j]);
				Mat I2;
				cv::resize(I, I2, I1.size());
				double scale = exp(-pow((double)(pinfo.radius[i] - pinfo_next.radius[j]), 2.0));
				double score = scale*getPSNR(I1, I2);
				
				friends_list.push_back(make_pair(j, score));
			}
			
		}
		friends_list.sort([](const pair<int, double>& _p1, const pair<int, double>& _p2){
			if(_p1.second > _p2.second)
				return true;
			return false;
		});
		
		vector<pair<int, double> > friends(friends_list.begin(), friends_list.end());
		pinfo.friends[i] = friends;
	}

	// calc global score
	double global_score = 0.0;
	for(int i = 1; i < pinfo.radius.size(); ++i) {
		if(pinfo.friends[i].size() > 0) {
			global_score += pinfo.friends[i][0].second;
		} else {
			global_score -= 20.0;
		}
	}

	if(pinfo.radius.size() > 1) {
		global_score /= pinfo.radius.size();
	} else {
		global_score = -100.0;
	}

	return global_score;
}

Mat MainWindow::drawImage() {
	Mat drawing;
	if(m_index < (m_particleinfos.size()-1) ) {
		cvtColor( m_cv_images[m_index], drawing, CV_GRAY2BGR );
		ParticlesInfo* pinfo = &m_particleinfos[m_index];
		ParticlesInfo* pinfo_next= &m_particleinfos[m_index+1];
		
		for(int i = 1; i < pinfo->radius.size(); i++) {
			Scalar color = Scalar(0, 0, 255);// rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    		if(m_check_show_polygon->isChecked()) 		drawContours(drawing, pinfo->contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    		if(m_check_show_boundingbox->isChecked()) 	rectangle(drawing, pinfo->boundRect[i].tl(), pinfo->boundRect[i].br(), color, 2, 8, 0 );
    		if(m_check_show_sphere->isChecked()) 		circle(drawing, pinfo->center[i], (int)pinfo->radius[i], color, 2, 8, 0 );
		}

		for(int i = 1; i < pinfo_next->radius.size(); i++) {
			Scalar color = Scalar( 0, 255, 255);//rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    		if(m_check_show_polygon_next->isChecked())		drawContours(drawing, pinfo_next->contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    		if(m_check_show_boundingbox_next->isChecked())	rectangle(drawing, pinfo_next->boundRect[i].tl(), pinfo_next->boundRect[i].br(), color, 2, 8, 0 );
    		if(m_check_show_sphere_next->isChecked())		circle(drawing, pinfo_next->center[i], (int)pinfo_next->radius[i], color, 2, 8, 0 );
		}

	
		if(m_check_show_radius->isChecked()) {
			for(int i = 1; i < pinfo->radius.size(); i++) {
    			circle(drawing, pinfo->center[i], m_radius, Scalar(255, 255, 255) , 2, 8, 0 );
			}
		}
		calcFriends(m_index, m_index+1);

		for(int i = 1; i < pinfo->friends.size(); ++i) {
			for(int j = 0; j < pinfo->friends[i].size(); ++j) {
				if(pinfo->friends[i][j].second < 10.0) {
					line(drawing, pinfo->center[i], pinfo_next->center[pinfo->friends[i][j].first], Scalar(255, 0, 0), 2, 8, 0);
				}
				else if(pinfo->friends[i][j].second < 20.0) {
					line(drawing, pinfo->center[i], pinfo_next->center[pinfo->friends[i][j].first], Scalar(255, 255, 0), 2, 8, 0);
				} else {
					line(drawing, pinfo->center[i], pinfo_next->center[pinfo->friends[i][j].first], Scalar(0, 255, 0), 2, 8, 0);
				}
			}
		}

		m_label_frame->setPixmap(QPixmap::fromImage(mat_to_qimage_ref(drawing, QImage::Format_RGB888).scaled(m_halfwidth, m_halfheight)));
	}

	return drawing;
}

ParticlesInfo MainWindow::getParticles(const cv::Mat& in, int thresh) {
	ParticlesInfo pinfo;

	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

  	/// Detect edges using Threshold
	threshold( in, threshold_output, thresh, 255, THRESH_BINARY );
	
	/// Find contours
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Approximate contours to polygons + get bounding rects and circles
	pinfo.contours_poly.resize( contours.size() );
	pinfo.boundRect.resize( contours.size() );
	pinfo.center.resize( contours.size() );
	pinfo.radius.resize( contours.size() );

	for( unsigned int i = 0; i < contours.size(); i++ ) { 
		approxPolyDP( Mat(contours[i]), pinfo.contours_poly[i], 3, true );
		pinfo.boundRect[i] = boundingRect( Mat(pinfo.contours_poly[i]) );
		minEnclosingCircle( (Mat)pinfo.contours_poly[i], pinfo.center[i], pinfo.radius[i] );
	}

	return pinfo;
}

// calculate the ssim
Scalar MainWindow::getMSSIM( const Mat& i1, const Mat& i2) {
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

double MainWindow::getPSNR(const Mat& I1, const Mat& I2) {
    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2

    Scalar s = sum(s1);        // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    if( sse <= 1e-10) // for small values return zero
        return 0;
    else {
        double mse  = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}
