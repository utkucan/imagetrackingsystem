#include "its.h"

ITS::ITS(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	ui.setupUi(this);
	QPixmap* icon = new QPixmap();
	icon->convertFromImage(*(new QImage("icon.png")));
	this->setWindowIcon(*icon);

	QPixmap* p = new QPixmap();
	p->convertFromImage(*(new QImage("LOGO2.jpg")),Qt::AutoColor);
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
	
	QTimer::singleShot(1000*10, this, SLOT(controlList(ip)));

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

	if(QDirectory.size()==0){
		return;
	
	}
//	photoPos = photoList->size();
	importPhotos* ip = new importPhotos(QDirectory,database,treeWidget->getPhotoList(),treeWidget->getFaceList(),m,rt);
	ipList.append(ip);
	ip->start();
	
	QTimer::singleShot(1000*10,this , SLOT(controlList()));

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

void ITS::controlList(){
	int count = 0;
	while(count<ipList.size()){
		if(ipList[count]->isRunning()){
			ipList[count]->lockProcess();
	//		displayPhoto(photoPos);
			//displayFace(photoPos);
	//		photoPos = photoList->size();
	//		treeWidget->reDoLastOperation();
	//		ipList[count]->wakeProcess();
			count++;
		}else{
			ipList.removeAt(count);
		}
	}
	if(ipList.size() != 0){
		treeWidget->reDoLastOperation();
		count = 0;
		while(count<ipList.size()){
			ipList[count]->wakeProcess();
			count++;
		}
		QTimer::singleShot(1000*10, this, SLOT(controlList()));
	}
	/*
	if(!ip->isFinished()){
		ip->lockProcess();
//		displayPhoto(photoPos);
		//displayFace(photoPos);
//		photoPos = photoList->size();
		treeWidget->reDoLastOperation();
		ip->wakeProcess();
		QTimer::singleShot(1000*3, this, SLOT(controlList(ip)));			
	}
	*/
}

void ITS::updateFace(){
	/*
	ranking* r = new ranking(database,treeWidget->getFaceList());
	r->start();
	*/
	if(!treeWidget->getUpdateFaces()){
		mdiArea->updateFace();
		treeWidget->updateTree();
		mdiArea->reDisplayLastPhoto();
		checkRecognizedFaces();
	}
	
	//treeWidget->reDoLastOperation();
 //   child->show();
}

void ITS::checkRecognizedFaces(){
	if(!treeWidget->getReDoFlag()){
		treeWidget->checkRecognizedFaces();
		//treeWidget->reDoLastOperation();
		if(rt->rankingRunning()){
			QTimer::singleShot(1000*10, this, SLOT(checkRecognizedFaces()));
		}
	}else
		QTimer::singleShot(1000*10, this, SLOT(checkRecognizedFaces()));
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
		ipList.append(ip);
		ip->start();
		QTimer::singleShot(1000*10, this, SLOT(controlList()));
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
void ITS::showAbout(){
	QString str ="Contact Info:\nBurak Uzun\tburak.1251@gmail.com\nEmre Sener\temre_sener@hotmail.com\nSercan Aksoy\tsrcnaks@gmail.com\nUtku Can Yucel\tutkucanyucel@gmail.com\nIbrahim Buyukgebiz\tibgebiz@gmail.com\n";
    QMessageBox::information(this,"About",str);
	

}