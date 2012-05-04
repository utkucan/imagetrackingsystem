#include "its.h"

ITS::ITS(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	QPixmap* p = new QPixmap();
	p->convertFromImage(*(new QImage("LOGO.jpg")),Qt::AutoColor);
	QSplashScreen* frame= new QSplashScreen(*p);
	frame->show();
	

	database = new db();
	rt = new rankingThread();
	mdiArea = new mdi(ui.mdiArea,database,rt);
	mdids = new mdiDS(ui.mdiDownSapmle, mdiArea,database);
	treeWidget = new treeWid(this,database,ui.treeWidget,mdids,mdiArea);
	m = new matlab();
	rt->initRanking(database,treeWidget->getFaceList());
	
	frame->close();

	importPhotos* ip = new importPhotos(QStringList(),database,treeWidget->getPhotoList(),treeWidget->getFaceList(),m,rt);
	ip->start();
	
	QTimer::singleShot(1000*3, this, SLOT(controlList(ip)));

	delete frame;


//	ui.setupUi(this);
	
//	photoList = new QList<photo*>();
	

	
/*	if(!database->openDB())
		exit(1);
*/

//	initializeTree();
//	faceCount = 1;
}

ITS::~ITS()
{

}
/*
void ITS::initializeTree(){
	unlabeledPhotoItem = new QTreeWidgetItem(treeWidget);
	QString phtotemp("Unlabeled Photos");
	unlabeledPhotoItem->setText(0,phtotemp);
	
	labelItem = new QTreeWidgetItem(treeWidget);
	QString temp("Labels");
	labelItem->setText(0,temp);
}
*/
/*
void ITS::displayPhoto(int pos){
	for(int i = pos; i< photoList->size(); i++){
		mdids->addMdiDSChild((*photoList)[i]);
	}
}

void ITS::displayFace(int pos){
	for(int i = pos; i< photoList->size(); i++){
		QList<face*>* faces = (*photoList)[i]->getFaces();
		for(int j = 0; j< faces->size(); j++){
			(*faces)[j]->setID(faceCount);
			faceCount++;
			mdiArea->addMdiFace((*faces)[j]);
		}
	}
}
*/

void ITS::on_actionKlasor_triggered(){

    QStringList QDirectory = QFileDialog::getOpenFileNames(
                         this,
                         "Select one or more files to open",
                         "",
                         "Images (*.jpg)");

	
//	photoPos = photoList->size();
	importPhotos* ip = new importPhotos(QDirectory,database,treeWidget->getPhotoList(),treeWidget->getFaceList(),m,rt);
	ip->start();
	
	QTimer::singleShot(1000*3, this, SLOT(controlList(ip)));

/*	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(controlList()));
	timer->start(100);
*/
/*
	QApplication::setOverrideCursor(Qt::WaitCursor);
	while(!ip->isFinished());
//	displayFace();
	displayPhoto(pos);
	QApplication::restoreOverrideCursor();
*/
	
}

void ITS::controlList(importPhotos* ip){
	if(!ip->isFinished()){
		ip->lockProcess();
//		displayPhoto(photoPos);
		//displayFace(photoPos);
//		photoPos = photoList->size();
		treeWidget->reDoLastOperation();
		ip->wakeProcess();
		QTimer::singleShot(1000*3, this, SLOT(controlList(ip)));			
	}
}

void ITS::updateFace(){
	/*
	ranking* r = new ranking(database,treeWidget->getFaceList());
	r->start();
	*/
	mdiArea->updateFace();
	treeWidget->updateTree();
	mdiArea->reDisplayLastPhoto();
	//treeWidget->reDoLastOperation();
 //   child->show();
}

bool ITS::event ( QEvent * e ){
	if(e->type() == QEvent::Move)
		mdiArea->setPosition();
	return QWidget::event(e);
}

void ITS::on_actionSearch_HardDisk_triggered(){
	QFileDialog dialog(this);

	dialog.setFileMode(QFileDialog::Directory);

	dialog.exec();
	QStringList QDirectory = dialog.selectedFiles();
	if(QDirectory.size()>0){
		importPhotos* ip = new importPhotos(QDirectory[0],database,treeWidget->getPhotoList(),treeWidget->getFaceList(),m,rt);
		ip->start();
		QTimer::singleShot(1000*3, this, SLOT(controlList(ip)));
	}
}
/*
void ITS::findImage(QString inp,QStringList* allImagesList){
//	QStringList allImagesList; //= new QList<QString>();
	QDir* direc = new QDir(inp);
	QFileInfoList fili = direc->entryInfoList();
	for(int i = 0 ; i < fili.size() ;i++){
		QFileInfo info = fili.at(i);
		if(info.fileName().compare(".")!=0 && info.fileName().compare("..")!=0){
			bool isDirec = info.isDir();
			if(isDirec){
				findImage(info.absoluteFilePath(),allImagesList);
			}
			else{
				QString suf = info.suffix();
				if(suf.compare("jpg")==0)
					allImagesList->append(info.absoluteFilePath());
			}
		}
	}
//	return allImagesList;
	direc->entr
	QFileSystemModel* fsm = new QFileSystemModel();
	fsm->setRootPath(inp);
	//bool a = fsm->isDir();
	return new QList<QString>();
}
*/
void ITS::treeWidgetSelectionChange(){
	treeWidget->selectedItemChange();

}