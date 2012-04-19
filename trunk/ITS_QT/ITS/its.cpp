#include "its.h"

ITS::ITS(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	mdiArea = new mdi(ui.mdiArea);
	mdids = new mdiDS(ui.mdiDownSapmle, mdiArea);
	photoList = new QList<photo*>();
	treeWidget = ui.treeWidget;

	database = new db();
/*	if(!database->openDB())
		exit(1);
*/

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


	photoPos = photoList->size();
	ip = new importPhotos(QDirectory,photoList);
	ip->start();
	
	QTimer::singleShot(1000*3, this, SLOT(controlList()));
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
	if(!ip->isFinished()){
			ip->lockProcess();
			displayPhoto(photoPos);
			photoPos = photoList->size();
			ip->wakeProcess();
			QTimer::singleShot(1000*3, this, SLOT(controlList()));			
		}
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
	QString* fileName = new QString();
	QStringList QDirectory = dialog.selectedFiles();
	allImagesList = new QList<QString>();
	findImage(QDirectory[0]);
	int a = 5;
}

void ITS::findImage(QString inp){
	QDir* direc = new QDir(inp);
	QFileInfoList fili = direc->entryInfoList();
	for(int i = 0 ; i < fili.size() ;i++){
		QFileInfo info = fili.at(i);
		if(info.fileName().compare(".")!=0 && info.fileName().compare("..")!=0){
			bool isDirec = info.isDir();
			if(isDirec){
				findImage(info.absoluteFilePath());
			}
			else{
				QString suf = info.suffix();
				if(suf.compare("jpg")==0)
					allImagesList->append(info.absoluteFilePath());
			}
		}
	}
	/*direc->entr
	QFileSystemModel* fsm = new QFileSystemModel();
	fsm->setRootPath(inp);
	//bool a = fsm->isDir();
	return new QList<QString>();*/
}
