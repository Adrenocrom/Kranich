#include "kranich.h"


using namespace cv;


RNG rng(12345);

MainWindow::MainWindow() {
	// labels
	m_label_frame  = nullptr;

	// widgets
	m_widget_stacked 	= nullptr;
	m_widget_empty		= nullptr;
	m_tableWidget_table = nullptr;
	
	m_active	= -1;
	m_radius 	= 50;
	m_threshold = 100;

	m_bilateral = 3;

	createMenu();
	createWidgets();

	setMinimumSize(1300, 768);
	setMaximumSize(1300, 768);

}

MainWindow::~MainWindow() {
	SAFE_DELETE(m_label_frame);
	SAFE_DELETE(m_widget_empty);
	SAFE_DELETE(m_widget_stacked);
	//SAFE_DELETE(m_tableWidget_table);
}

void MainWindow::createMenu() {
	QMenu* menu_file = menuBar()->addMenu(tr("&File"));
	
	m_action_import = new QAction(tr("import"), this);
	menu_file->addAction(m_action_import);
	menu_file->addSeparator();

	connect(m_action_import, &QAction::triggered, this, &MainWindow::importImages);

	//QAction* action_quit = menu_file->addAction(tr("&Quit"), this, &QWidget::close);
   	//action_quit->setShortcuts(QKeySequence::Quit);
	//action_quit->setStatusTip(tr("Quit the application"));
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


	m_line_min_speed = new QLineEdit("0.0");
	m_line_max_speed = new QLineEdit("200.0");
	connect(m_line_max_speed, &QLineEdit::textChanged, this, &MainWindow::changeRadius);

	m_label_filename = new QLabel("filename: ");

	m_button_save = new QPushButton(tr("save"));
	connect(m_button_save, &QPushButton::pressed, this, &MainWindow::saveImage);

	gridLayout->addWidget(m_label_frame, 0, 0, 1, 10);
	gridLayout->addWidget(m_label_filename, 1, 0, 1, 8);
	gridLayout->addWidget(m_button_save, 1, 8, 1, 2);
	gridLayout->addWidget(m_slider_images, 2, 0, 1, 10);
	gridLayout->addWidget(new QLabel("threshold"), 3, 0, 1, 1);
	gridLayout->addWidget(m_slider_threshold, 3, 1, 1, 8);
	gridLayout->addWidget(m_label_threshold, 3, 9, 1, 1);
	gridLayout->addWidget(new QLabel("min speed [m/s]"), 4, 0, 1, 1);
	gridLayout->addWidget(m_line_min_speed, 4, 1, 1, 1);
	gridLayout->addWidget(new QLabel("max speed [m/s]"), 4, 4, 1, 1);
	gridLayout->addWidget(m_line_max_speed, 4, 5, 1, 1);

	QGroupBox* gb = new QGroupBox(tr(""));
	QHBoxLayout* hbox2 = new QHBoxLayout;

	m_check_show_radius			= new QCheckBox(tr("show radius"));
	m_check_show_radius->setChecked(true);
	connect(m_check_show_radius, &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	
	m_check_show_links = new QCheckBox(tr("show links"));
	m_check_show_links->setChecked(true);
	connect(m_check_show_links,	&QCheckBox::stateChanged, this, &MainWindow::drawImage);

	m_check_show_links_selected	= new QCheckBox(tr("selected links"));
	connect(m_check_show_links_selected, &QCheckBox::stateChanged, this, &MainWindow::drawImage);

	hbox2->addWidget(m_check_show_radius);
	hbox2->addWidget(m_check_show_links);
	hbox2->addWidget(m_check_show_links_selected);
	gb->setLayout(hbox2);
	gridLayout->addWidget(gb, 5, 0, 1, 10);

	m_groupbox_frame = new QGroupBox(tr("this frame"));

	m_check_show_boundingbox 	= new QCheckBox(tr("show bounding box"));
	m_check_show_sphere 		= new QCheckBox(tr("show sphere"));
	m_check_show_polygon 		= new QCheckBox(tr("show polygon"));
	
	m_check_show_boundingbox->setChecked(true);
	m_check_show_sphere->setChecked(true);
	m_check_show_polygon->setChecked(true);

	connect(m_check_show_boundingbox, &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	connect(m_check_show_sphere, 	  &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	connect(m_check_show_polygon, 	  &QCheckBox::stateChanged, this, &MainWindow::drawImage);
	
	QVBoxLayout *vbox = new QVBoxLayout;
  	vbox->addWidget(m_check_show_boundingbox);
  	vbox->addWidget(m_check_show_sphere);
  	vbox->addWidget(m_check_show_polygon);
	
	//vbox->addStretch(1);
	m_groupbox_frame->setLayout(vbox);
	gridLayout->addWidget(m_groupbox_frame, 6, 0, 1, 5);


	m_groupbox_frame_next = new QGroupBox(tr("next frame"));

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
	m_groupbox_frame_next->setLayout(vboxN);
	gridLayout->addWidget(m_groupbox_frame_next, 6, 5, 1, 5 );

	QGroupBox* gb2 = new QGroupBox(tr("infos"));
	QFormLayout* fl2 = new QFormLayout;

	m_line_date 			 			= new QLineEdit("");
	m_line_abrasive_material 			= new QLineEdit("");
	m_line_particle_size_distribution 	= new QLineEdit("");
	m_line_blast_cabinet_pressure 		= new QLineEdit("");
	m_line_sample_identifier 			= new QLineEdit("");
	m_line_experiment_label 			= new QLineEdit("");
	m_line_revolutions_per_minute 		= new QLineEdit("");
	m_line_flow 						= new QLineEdit("");
	m_line_particle_flow 				= new QLineEdit("");

	fl2->addRow(new QLabel("date"), m_line_date);
	fl2->addRow(new QLabel("abrasive material"), m_line_abrasive_material);
	fl2->addRow(new QLabel("particle size distribution"), m_line_particle_size_distribution);
	fl2->addRow(new QLabel("blast cabinet pressure [bar]"), m_line_blast_cabinet_pressure);
	fl2->addRow(new QLabel("sample identifier"), m_line_sample_identifier);
	fl2->addRow(new QLabel("experiment label"), m_line_experiment_label);
	fl2->addRow(new QLabel("revolutions per minute [U/min]"), m_line_revolutions_per_minute);
	fl2->addRow(new QLabel("flow [l/min]"), m_line_flow);
	fl2->addRow(new QLabel("particle flow [g/min]"), m_line_particle_flow);

	gb2->setLayout(fl2);
	gridLayout->addWidget(gb2, 0, 11, 1, 1);

	QGroupBox*	gb3 = new QGroupBox(tr("scale factors"));
	QFormLayout* fl3 = new QFormLayout;

	m_line_scale_time		= new QLineEdit("1.0");
	m_line_scale_distance	= new QLineEdit("1.0");
	m_line_scale_density	= new QLineEdit("1.0");
	m_button_export			= new QPushButton(tr("export"));
	m_button_export->setMinimumWidth(500);
	connect(m_button_export, &QPushButton::pressed, this, &MainWindow::evaluate);
	connect(m_line_scale_distance, &QLineEdit::textChanged, this, &MainWindow::changeRadius);
	connect(m_line_scale_time, &QLineEdit::textChanged, this, &MainWindow::changeRadius);

	fl3->addRow(new QLabel("time ["+QString(QChar(0x03BC))+"s]"), m_line_scale_time);
	fl3->addRow(new QLabel("distance ["+QString(QChar(0x03BC))+"m/pixel]"), m_line_scale_distance);
	fl3->addRow(new QLabel("density [g/cm"+QString(QChar(0x00B3))+"]"), m_line_scale_density);
	fl3->addRow(new QLabel(""));
	fl3->addRow(new QLabel(""));
	fl3->addRow(new QLabel(""));
	fl3->addRow(new QLabel(""));
	fl3->addRow(new QLabel(""));
	fl3->addRow(m_button_export);

	gb3->setLayout(fl3);
	gridLayout->addWidget(gb3, 1, 11, 6, 1);

	
	m_tableWidget_table = new QTableWidget();
	m_tableWidget_table->setColumnCount(1);
  	m_tableWidget_table->setColumnWidth(0,  200);
	
	QStringList header;
	header << "radius in px" ;
	m_tableWidget_table->setHorizontalHeaderLabels(header);
	m_tableWidget_table->setMinimumWidth(200);
	//m_tableWidget_table->verticalHeader()->setVisible(false);
	m_tableWidget_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	connect(m_tableWidget_table, &QTableWidget::cellDoubleClicked, this, &MainWindow::setParticleActive);
	
	gridLayout->addWidget(m_tableWidget_table, 0, 12, 8, 1);

	m_widget_main->setLayout(gridLayout);
	m_widget_stacked->addWidget(m_widget_main);
}

void MainWindow::importImages() {
	QStringList filenames = QFileDialog::getOpenFileNames(this,tr("BMP files"),QDir::currentPath(),tr("Bitmap files (*.bmp);;All files (*.*)") );
	m_cv_images.clear();
	m_particleinfos.clear();
	m_filenames.clear();
	m_active = -1;
	if( !filenames.isEmpty() ) {
		m_widget_stacked->setCurrentIndex(KN_WIDGET_LOAD);
		this->repaint();	
		m_progress_load->setRange(0, filenames.count()-1);
		m_progress_load->setValue(0);

		filenames.sort();
	
		m_cv_images.resize(filenames.count());
		m_particleinfos.resize(filenames.count());
		m_filenames.resize(filenames.count());
		for(int i = 0; i < filenames.count(); i++) {
			string filename = filenames.at(i).toStdString();
			size_t found = filename.find_last_of("/\\");
			m_filenames[i] = filename.substr(found+1);

			m_progress_load->setValue(i);
			
			Mat src = imread(filenames.at(i).toStdString(), 1);
			//Mat _dest;
			//src.convertTo(_dest, CV_8U);
			cvtColor( src, src, CV_BGR2GRAY );
   			blur( src, m_cv_images[i], Size(3,3) );
			//bilateralFilter ( src, m_cv_images[i], m_bilateral, m_bilateral*2, m_bilateral/2 );

			m_particleinfos[i] = new ParticlesInfo;
			getParticles(m_cv_images[i], m_particleinfos[i]);
		}
		

		m_halfwidth  = (int)((float)m_cv_images[0].cols / 2.0f);
		m_halfheight = (int)((float)m_cv_images[0].rows / 2.0f);
			
		m_slider_images->setMaximum(filenames.count()-1);
		m_slider_images->setValue(0);
		m_index = 0;
		m_line_min_speed->setText("0.0");
		m_line_max_speed->setText("200.0");
		m_label_filename->setText(QString("filename " + QString::fromStdString(m_filenames[m_index])));
		m_groupbox_frame->setTitle(QString("this frame " + QString::fromStdString(m_filenames[m_index])));
		m_groupbox_frame_next->setTitle(QString("next frame " + QString::fromStdString(m_filenames[m_index+1])));

		m_line_date->setText( QDateTime::currentDateTime().toString() );


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

void MainWindow::setParticleActive(int r, int c) {
	m_active = r+1;
	drawImage();
}

void MainWindow::changeThreshold(int value) {
	m_threshold = value;
	m_label_threshold->setText(QString::number(value));
	drawImage();
}

void MainWindow::changeRadius(QString value) {
	double speed = m_line_max_speed->text().toDouble();
	double scale = m_line_scale_distance->text().toDouble();
	if(scale > 0.0)
		speed *= m_line_scale_time->text().toDouble() / scale;
	
	m_radius = (int)speed;
	drawImage();
}

void MainWindow::changeImage(int value) {
	m_index	= value;
	m_label_filename->setText(QString("filename: " + QString::fromStdString(m_filenames[m_index])));
	m_groupbox_frame->setTitle(QString("this frame " + QString::fromStdString(m_filenames[m_index])));
	if(m_index+1 < (int)(m_filenames.size()) )
		m_groupbox_frame_next->setTitle(QString("next frame " + QString::fromStdString(m_filenames[m_index+1])));
	m_active = -1;
	drawImage();
}

void MainWindow::saveImage() {
	QString filename = QFileDialog::getSaveFileName(this,tr("BMP files"),QDir::currentPath(),tr("BMP files (*.bmp);;All files (*.*)") );
	if(filename.toStdString() != "")
		imwrite(filename.toStdString(), m_frame);
}

void MainWindow::evaluate() {
	int _size = (int) m_cv_images.size();
	for(int i = 0; i < _size; ++i) {
		getParticles(m_cv_images[i], m_particleinfos[i], m_threshold);
	}

	QString filename = QFileDialog::getSaveFileName(this,tr("CSV files"),QDir::currentPath(),tr("CSV files (*.csv);;All files (*.*)") );
	if(filename == "") return;

	double	scale_time		= m_line_scale_time->text().toDouble();
	double	scale_distance	= m_line_scale_distance->text().toDouble();
	double	scale_density	= m_line_scale_density->text().toDouble();


	ofstream file;
  	file.open(filename.toStdString());
	file<<"date"<<","<<m_line_date->text().toStdString()<<"\n";
	file<<"abrasive material"<<","<<m_line_abrasive_material->text().toStdString()<<"\n";
	file<<"blast cabinet pressure [bar]"<<","<<m_line_blast_cabinet_pressure->text().toStdString()<<"\n";
	file<<"sample identifier"<<","<<m_line_sample_identifier->text().toStdString()<<"\n";
	file<<"experiment label"<<","<<m_line_experiment_label->text().toStdString()<<"\n";
	file<<"revolutions per minute [U/min]"<<","<<m_line_revolutions_per_minute->text().toStdString()<<"\n";
	file<<"flow [l/min]"<<","<<m_line_flow->text().toStdString()<<"\n";
	file<<"particle flow [g/min]"<<","<<m_line_particle_flow->text().toStdString()<<"\n";
	file<<"threshold"<<","<<m_threshold<<"\n";
	file<<"radius"<<","<<m_radius<<"\n";
	file<<"scale distance [\u00B5m/px]"<<","<<scale_distance<<"\n";
	file<<"scale time [\u00B5s]"<<","<<scale_time<<"\n";
	file<<"min speed [m/s]"<<","<<m_line_min_speed->text().toStdString()<<"\n";
	file<<"max speed [m/s]"<<","<<m_line_max_speed->text().toStdString()<<"\n";
	file<<"id,radius [\u00B5m],volume [\u00B5m\u00B3],density [g/cm\u00B3],mass [\u00B5g],velocity [m/s],E_kin[\u00B5J],score,file1,file2\n";

	int size 				= m_cv_images.size() - 1;
	int cnt					= 0;
	double	mean_radius		= 0.0;
	double	volume			= 0.0;
	double  density			= 0.0;
	double	mass			= 0.0;
	double	distance		= 0.0;
	double	velocity		= 0.0;
	double	E_kin			= 0.0;
	double	score			= 0.0;
	double	min_velocity	= m_line_min_speed->text().toDouble();
	int 	_size_a			= 0;
	for(int i = 0; i < size; ++i) {
		score = calcFriends(i, i+1);
		cout<<"score: "<<score<<endl;
		if(score > -1.0) {
			ParticlesInfo* pinfo 	  = m_particleinfos[i];
			ParticlesInfo* pinfo_next = m_particleinfos[i+1];

			_size_a = (int) pinfo->radius.size();
			for(int a = 0; a < _size_a; ++a) {
				if(pinfo->friends[a].size() > 0) {
					mean_radius = ((double) pinfo->radius[a] + (double) pinfo_next->radius[pinfo->friends[a][0].first]) / 2.0;
					mean_radius *= scale_distance;
					volume = (4.0/3.0) * KN_PI * pow(mean_radius, 3.0);
					density= scale_density;
					mass   = density * (volume * 1e-12);

					//distance = norm((pinfo->center[a] * scale_distance)  - (pinfo_next->center[pinfo->friends[a][0].first] * scale_distance));
					distance = norm(pinfo->center[a]  - pinfo_next->center[pinfo->friends[a][0].first]);
					distance*= scale_distance;
					velocity = distance / scale_time;
					mass  *= 1e-3;
					E_kin  = 0.5 * mass * pow(velocity, 2.0);
					E_kin *= 1e+6; 
					mass  *= 1e+9;
					if(pinfo->center[a].x > pinfo_next->center[pinfo->friends[a][0].first].x)
						E_kin *= -1;
					
					if(velocity > min_velocity) {
						file<< cnt <<","<< mean_radius <<","<< volume <<","<< density <<","<< mass << "," << velocity << "," << E_kin << "," << score << "," << m_filenames[i] << "," << m_filenames[i+1] <<"\n";
						cnt++;
					}
				}
			}
		}
	}
}
 
double MainWindow::calcFriends(int a, int b) {
	ParticlesInfo* pinfo 		= m_particleinfos[a];
	ParticlesInfo* pinfo_next 	= m_particleinfos[b];
	pinfo->friends.resize(pinfo->radius.size());
	int _size_a = (int) pinfo->radius.size();
	int _size_b = (int) pinfo_next->radius.size();

	for(int i = 1; i < _size_a; ++i) {
		list<pair<int, double> > friends_list;

		for(int j = 1; j < _size_b; ++j) {
			Point2f p = pinfo->center[i] - pinfo_next->center[j];

			if(norm(p) < (double)m_radius) {
				Mat I1 = m_cv_images[a](pinfo->boundRect[i]);
				Mat I  = m_cv_images[b](pinfo_next->boundRect[j]);
				Mat I2;
				cv::resize(I, I2, I1.size());
				double scale = exp(-pow((double)(pinfo->radius[i] - pinfo_next->radius[j]), 4.0));
				double score = scale*getPSNR(I1, I2);
				cout<<"s "<<scale<<endl;
				friends_list.push_back(make_pair(j, score));
			}
			
		}
		friends_list.sort([](const pair<int, double>& _p1, const pair<int, double>& _p2){
			if(_p1.second > _p2.second)
				return true;
			return false;
		});
		
		vector<pair<int, double> > friends(friends_list.begin(), friends_list.end());
		pinfo->friends[i] = friends;
	}

	// calc global score
	double global_score = 0.0;
	for(int i = 1; i < _size_a; ++i) {
		if(pinfo->friends[i].size() > 0) {
			global_score += pinfo->friends[i][0].second;
		} else {
			global_score -= 20.0;
		}
	}

	if(pinfo->radius.size() > 1) {
		global_score /= pinfo->radius.size();
	} else {
		global_score = -100.0;
	}

	return global_score;
}

Mat MainWindow::drawImage() {
	Mat drawing;

	if(m_index < (int)(m_particleinfos.size()-1) ) {
		cvtColor( m_cv_images[m_index], drawing, CV_GRAY2BGR );

		getParticles(m_cv_images[m_index], 	 m_particleinfos[m_index],   m_threshold);
		getParticles(m_cv_images[m_index+1], m_particleinfos[m_index+1], m_threshold);

		ParticlesInfo* pinfo 	 = m_particleinfos[m_index];
		ParticlesInfo* pinfo_next= m_particleinfos[m_index+1];
		int _size_a = (int) pinfo->radius.size();
		int _size_b = (int) pinfo_next->radius.size();
		
		if(_size_a > 1) m_tableWidget_table->setRowCount(_size_a-1);
		else {
			m_tableWidget_table->setRowCount(0);
		}
		
		for(int i = 1; i < _size_a; i++) {
			Scalar color = Scalar(0, 0, 255);// rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    		if(m_check_show_polygon->isChecked()) 		drawContours(drawing, pinfo->contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    		if(m_check_show_boundingbox->isChecked()) 	rectangle(drawing, pinfo->boundRect[i].tl(), pinfo->boundRect[i].br(), color, 2, 8, 0 );
    		if(m_check_show_sphere->isChecked()) 		circle(drawing, pinfo->center[i], (int)pinfo->radius[i], color, 2, 8, 0 );
		
			m_tableWidget_table->setItem(i-1, 0, new QTableWidgetItem(QString::number(pinfo->radius[i])));
		}

		for(int i = 1; i < _size_b; i++) {
			Scalar color = Scalar( 0, 255, 255);//rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    		if(m_check_show_polygon_next->isChecked())		drawContours(drawing, pinfo_next->contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    		if(m_check_show_boundingbox_next->isChecked())	rectangle(drawing, pinfo_next->boundRect[i].tl(), pinfo_next->boundRect[i].br(), color, 2, 8, 0 );
    		if(m_check_show_sphere_next->isChecked())		circle(drawing, pinfo_next->center[i], (int)pinfo_next->radius[i], color, 2, 8, 0 );
		}

		if(m_check_show_radius->isChecked()) {
			for(int i = 1; i < _size_a; i++) {
    			circle(drawing, pinfo->center[i], m_radius, Scalar(255, 255, 255) , 2, 8, 0 );
			}
		}
		
		if(m_active >= _size_a) m_active = _size_a-1;
		if(_size_a == 1) m_active = -1;
		if(m_active > -1) {
			Scalar color = Scalar(0, 255, 0);// rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			if(m_check_show_radius->isChecked()) 		circle(drawing, pinfo->center[m_active], m_radius, color , 2, 8, 0 );
    		if(m_check_show_polygon->isChecked()) 		drawContours(drawing, pinfo->contours_poly, m_active, color, 1, 8, vector<Vec4i>(), 0, Point() );
    		if(m_check_show_boundingbox->isChecked()) 	rectangle(drawing, pinfo->boundRect[m_active].tl(), pinfo->boundRect[m_active].br(), color, 2, 8, 0 );
    		if(m_check_show_sphere->isChecked()) 		circle(drawing, pinfo->center[m_active], (int)pinfo->radius[m_active], color, 2, 8, 0 );

		}

		
		calcFriends(m_index, m_index+1);

		if(m_check_show_links->isChecked()) {
			int _size_f = pinfo->friends.size();
			for(int i = 1; i < _size_f; ++i) {
				for(int j = 0; j < (int) pinfo->friends[i].size(); ++j) {
					if(m_check_show_links_selected->isChecked() && j == 0) {
						line(drawing, pinfo->center[i], pinfo_next->center[pinfo->friends[i][j].first], Scalar(255, 255, 255), 4, 8, 0);
					} else {
						if(pinfo->friends[i][j].second < 10.0) line(drawing, pinfo->center[i], pinfo_next->center[pinfo->friends[i][j].first], Scalar(255, 0, 0), 2, 8, 0);
						else if(pinfo->friends[i][j].second < 20.0) line(drawing, pinfo->center[i], pinfo_next->center[pinfo->friends[i][j].first], Scalar(255, 255, 0), 2, 8, 0);
						else line(drawing, pinfo->center[i], pinfo_next->center[pinfo->friends[i][j].first], Scalar(0, 255, 0), 2, 8, 0);
					}
				}
			}
		}
		
		cvtColor(drawing, m_frame, CV_BGR2RGB);
		m_label_frame->setPixmap(QPixmap::fromImage(mat_to_qimage_ref(drawing, QImage::Format_RGB888).scaled(m_halfwidth, m_halfheight)));
	}

	return drawing;
}

void MainWindow::getParticles(const cv::Mat& in, ParticlesInfo* pinfo, int thresh) {
	if(pinfo) {
		Mat threshold_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		/// Detect edges using Threshold
		//threshold( in, threshold_output, thresh, 255, THRESH_BINARY );

		adaptiveThreshold(in, threshold_output, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 151, thresh);
		//Canny( in, threshold_output, thresh, thresh*2, 3 );
		/// Find contours
		findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		/// Approximate contours to polygons + get bounding rects and circles
		pinfo->contours_poly.resize( contours.size() );
		pinfo->boundRect.resize( contours.size() );
		pinfo->center.resize( contours.size() );
		pinfo->radius.resize( contours.size() );

		for( unsigned int i = 0; i < contours.size(); i++ ) { 
			approxPolyDP( Mat(contours[i]), pinfo->contours_poly[i], 3, true );
			pinfo->boundRect[i] = boundingRect( Mat(pinfo->contours_poly[i]) );
			minEnclosingCircle( (Mat)pinfo->contours_poly[i], pinfo->center[i], pinfo->radius[i] );
		}
	}
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
