#include "kranich.h"

MainWindow::MainWindow() {
	createMenu();

	QStringList filenames = QFileDialog::getOpenFileNames(this,tr("BMP files"),QDir::currentPath(),tr("Bitmap files (*.bmp);;All files (*.*)") );
	if( !filenames.isEmpty() ) {
		filenames.sort();
    	for (int i =0;i<filenames.count();i++)
    	    cout<<filenames.at(i).toStdString()<<endl;
	}
}

MainWindow::~MainWindow() {
	
}

void MainWindow::createMenu() {
	QMenu* menu_file = menuBar()->addMenu(tr("&File"));
	
	QAction* action_quit = menu_file->addAction(tr("&Quit"), this, &QWidget::close);
   	action_quit->setShortcuts(QKeySequence::Quit);
	action_quit->setStatusTip(tr("Quit the application"));
}
