#include "its.h"

ITS::ITS(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	mdiArea = new mdi(ui.mdiArea);
	photoList = new QList<photo*>();

	faceCount = 1;
}

ITS::~ITS()
{

}


void ITS::displayFace(){
	for(int i = 0; i< photoList->size(); i++){
		QList<face*>* faces = (*photoList)[i]->getFaces();
		for(int j = 0; j< faces->size(); j++){
			(*faces)[j]->setID(faceCount);
			faceCount++;
			mdiArea->addMdiChild((*faces)[j]);
		}
	}
}


void ITS::on_actionKlasor_triggered(){
	QListWidgetItem *item;
    QCheckBox *cbox;
    
    QStringList QDirectory = QFileDialog::getOpenFileNames(
                         this,
                         "Select one or more files to open",
                         "",
                         "Images (*.jpg)");

	importPhotos* ip = new importPhotos(QDirectory,photoList);
	ip->start();
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	while(!ip->isFinished());
	displayFace();
	QApplication::restoreOverrideCursor();
	
}