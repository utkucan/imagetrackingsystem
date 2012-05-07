#include "its.h"

ITS::ITS(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	ui.setupUi(this);
	QPixmap* icon = new QPixmap();
	icon->convertFromImage(*(new QImage("icon.png")));
	this->setWindowIcon(*icon);
	QPixmap* p = new QPixmap();
	p->convertFromImage(*(new QImage("LOGO2.png")));
	QSplashScreen* frame= new QSplashScreen(*p);
	frame->show();
	
	rti = 3;
	iti = 3;
	importTimer = new QTimer();
	recognizedTimer = new QTimer();
	database = new db();
	rt = new rankingThread();
	mdiArea = new mdi(ui.mdiArea,database,rt);
	mdids = new mdiDS(ui.mdiDownSapmle, mdiArea,database);
	treeWidget = new treeWid(this,database,ui.treeWidget,mdids,mdiArea);
	m = new matlab();
	rt->initRanking(database,treeWidget->getFaceList());

	frame->close();
	delete frame;

	importPhotos* ip = new importPhotos(QStringList(),database,treeWidget->getPhotoList(),treeWidget->getFaceList(),m,rt);
	ipList.append(ip);
	ip->start();
	importTimer->singleShot(1000*iti, this, SLOT(controlList()));

	
}

ITS::~ITS()
{

}

void ITS::on_actionKlasor_triggered(){

    QStringList QDirectory = QFileDialog::getOpenFileNames(
                         this,
                         "Select one or more files to open",
                         "",
                         "Images (*.jpg)");

	if(QDirectory.size()==0){
		return;
	
	}
	importPhotos* ip = new importPhotos(QDirectory,database,treeWidget->getPhotoList(),treeWidget->getFaceList(),m,rt);
	ipList.append(ip);
	ip->start();
	
	importTimer->singleShot(1000*iti,this , SLOT(controlList()));
}

void ITS::controlList(){
	int count = 0;
	while(count<ipList.size()){
		if(ipList[count]->isRunning()){
			ipList[count]->lockProcess();
			count++;
		}else{
			ipList.removeAt(count);
		}
	}
	if(ipList.size() != 0){
	//	treeWidget->reDoLastOperation();
		count = 0;
		while(count<ipList.size()){
			ipList[count]->wakeProcess();
			count++;
		}
		rt->start();
		recognizedTimer->singleShot(1000*rti, this, SLOT(checkRecognizedFaces()));
		importTimer->singleShot(1000*iti, this, SLOT(controlList()));
	}
}

void ITS::closeEvent(QCloseEvent *event){
	rt->stopThread();
	while(ipList.size()>0){
		if(ipList[0]->isRunning()){
			ipList[0]->setStop(true);
			while(ipList[0]->isRunning());
			ipList.removeAt(0);
		}else{
			ipList.removeAt(0);
		}
	}
	/*
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
	*/
}

void ITS::updateFace(){
	if(!treeWidget->getUpdateFaces()){
		treeWidget->updateTree();
		mdiArea->reDisplayLastPhoto();

		if(!recognizedTimer->isActive())
			recognizedTimer->singleShot(1000*rti, this, SLOT(checkRecognizedFaces()));
		//	recognizedTimer->stop();
		//checkRecognizedFaces();
	//	if(rt->getFound() || !recognizedTimer->isActive() )//!recognizedTimer->isActive())
			
		mdiArea->updateFace();

//		checkRecognizedFaces();
//		treeWidget->checkRecognizedFaces();
		
	}
	
}

void ITS::checkRecognizedFaces(){
	if(!treeWidget->getReDoFlag()){
		if(rt->getFound())
			treeWidget->checkRecognizedFaces();
		if(!rt->rankingRunning()){
			recognizedTimer->singleShot(1000*rti, this, SLOT(checkRecognizedFaces()));
		}
	}else
		recognizedTimer->singleShot(1000, this, SLOT(checkRecognizedFaces()));
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
		importTimer->singleShot(1000*iti, this, SLOT(controlList()));
	}
}

void ITS::treeWidgetSelectionChange(){
	treeWidget->selectedItemChange();
}
void ITS::showAbout(){
	QString str ="Contact Info:\nBurak Uzun\tburak.1251@gmail.com\nEmre Sener\temre_sener@hotmail.com\nSercan Aksoy\tsrcnaks@gmail.com\nUtku Can Yucel\tutkucanyucel@gmail.com\nIbrahim Buyukgebiz\tibgebiz@gmail.com\n";
    QMessageBox::information(this,"About",str);
}