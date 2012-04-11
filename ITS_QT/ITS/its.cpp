#include "its.h"

ITS::ITS(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	

	mdiArea = new mdi(ui.mdiArea);
	mdids = new mdiDS(ui.mdiDownSapmle, mdiArea);
	photoList = new QList<photo*>();
	treeWidget = ui.treeWidget;
	initializeTree();
	faceCount = 1;
}

ITS::~ITS()
{

}

void ITS::initializeTree(){
	photoItem = new QTreeWidgetItem(treeWidget);
	QString phtotemp("Photos");
	photoItem->setText(0,phtotemp);
	
	labelItem = new QTreeWidgetItem(treeWidget);
	QString temp("Labels");
	labelItem->setText(0,temp);
}

void ITS::displayPhoto(int pos){
	for(int i = pos; i< photoList->size(); i++){
		mdids->addMdiDSChild((*photoList)[i]);
	}
}

void ITS::displayFace(){
	for(int i = 0; i< photoList->size(); i++){
		QList<face*>* faces = (*photoList)[i]->getFaces();
		for(int j = 0; j< faces->size(); j++){
			(*faces)[j]->setID(faceCount);
			faceCount++;
			mdiArea->addMdiFace((*faces)[j]);
		}
	}
}


void ITS::on_actionKlasor_triggered(){
    
    QStringList QDirectory = QFileDialog::getOpenFileNames(
                         this,
                         "Select one or more files to open",
                         "",
                         "Images (*.jpg)");




	for( int i = 0; i<QDirectory.size(); i++){
		
		QTreeWidgetItem *subItem = new QTreeWidgetItem();
		photoItem->addChild(subItem);
		QString imageName = QFileInfo(QDirectory[i]).fileName();
		subItem->setText(0,imageName);
	}

	int pos = photoList->size();
	importPhotos* ip = new importPhotos(QDirectory,photoList);
	ip->start();
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	while(!ip->isFinished());
//	displayFace();
	displayPhoto(pos);
	QApplication::restoreOverrideCursor();
	
}