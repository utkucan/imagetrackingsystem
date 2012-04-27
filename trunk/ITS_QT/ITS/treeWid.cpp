#include "treeWid.h"


treeWid::treeWid(QWidget* prnt,db* database,QTreeWidget* treeWidge,mdiDS* mdids,mdi* mdiArea)
{
	this->database = database;
	this->treeWidget = treeWidge;
	this->mdids = mdids;
	this->mdiArea = mdiArea;
	itemList = new QList<QTreeWidgetItem*>();
	lastOperation = -1;
	listPos = 0;
	reDoFlag = false;

	photoList = new QList<photo*>();
	faceList = new QList<face*>();

	buildTree();
	buildLists();

	/*
	for( int i = 0; i<QDirectory.size(); i++){
		
		QTreeWidgetItem *subItem = new QTreeWidgetItem();
		photoItem->addChild(subItem);
		QString imageName = QFileInfo(QDirectory[i]).fileName();
		subItem->setText(0,imageName);
	}
	*/
}


treeWid::~treeWid(void)
{
}

void treeWid::updateTree(){
	QStringList personList = database->getAllPerson();
	if(size < personList.size()){
		for( int i = size; i<personList.size();i++){
			QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
			item->setText(0,personList[i]);
			//item->setCheckState(0,Qt::Unchecked);

			QTreeWidgetItem *subPhotoItem = new QTreeWidgetItem();
			subPhotoItem->setText(0,"Photos");
			item->addChild(subPhotoItem);

			QTreeWidgetItem *subFaceItem = new QTreeWidgetItem();
			subFaceItem->setText(0,"Faces");
			item->addChild(subFaceItem);

			itemList->append(item);
		}
		size = personList.size();
	}

}

void treeWid::buildTree(){
	QStringList personList = database->getAllPerson();
	size = personList.size();
	for( int i = 0; i<personList.size();i++){
		QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
		item->setText(0,personList[i]);
		//item->setCheckState(0,Qt::Unchecked);

		QTreeWidgetItem *subPhotoItem = new QTreeWidgetItem();
		subPhotoItem->setText(0,"Photos");
		item->addChild(subPhotoItem);

		QTreeWidgetItem *subFaceItem = new QTreeWidgetItem();
		subFaceItem->setText(0,"Faces");
		item->addChild(subFaceItem);

		itemList->append(item);
	}
}

void treeWid::buildLists(){
	database->getAllPhotos(photoList,faceList);
}

void treeWid::selectedItemChange(){
	QString selection(treeWidget->currentItem()->text(0));
	if(selection == "Photos" || selection == "Faces"){
		reDoFlag = false;
		personName = treeWidget->currentItem()->parent()->text(0);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if(selection == "Photos" ){
			lastOperation = operation::photoOperation;
			QList<int>* photoIDList = database->selectPersonPhoto(personName);
			displayPhoto(photoIDList);	
		}else{
			lastOperation = operation::faceOperation;
			QList<int>* faceIdList = database->selectPersonFace(personName);
			displayFace(faceIdList);
		}
		QApplication::restoreOverrideCursor();
	}else{
		if(treeWidget->currentItem()->isExpanded())
			treeWidget->collapse(treeWidget->currentIndex());
		else
			treeWidget->expand(treeWidget->currentIndex());
	}
	
//	QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
//	QString personName(items[0]->parent()->text(0));
//	QList<photo*>* photoList = database->selectPhoto(personName);
//	displayPhoto(photoList);
//	QString s(treeWidget->currentItem()->text(0));
//	int a = 5;
}

void treeWid::reDoLastOperation(){
	if(lastOperation != -1){
		reDoFlag = true;
		if(lastOperation == operation::photoOperation){
			QList<int>* photoIdList = database->selectPersonPhoto(personName);
			displayPhoto(photoIdList);
		}else{
			QList<int>* faceIdList = database->selectPersonFace(personName);
			displayFace(faceIdList);
		}
	}
}

void treeWid::displayPhoto(QList<int>* photoIdList){
	if(!reDoFlag){
		mdids->clearMdiDS();
		listPos = 0;
	}
	for(int i = listPos; i< photoIdList->size(); i++){
		mdids->addMdiDSChild((*photoList)[(*photoIdList)[i]-1]);
	}
	listPos = photoIdList->size();
}

void treeWid::displayFace(QList<int>* faceIdList){
	if(!reDoFlag){
		mdids->clearMdiDS();
		listPos = 0;
	}
	for(int i = listPos; i< faceIdList->size(); i++){
		mdiArea->addMdiFace((*faceList)[(*faceIdList)[i]-1]);
	}
	listPos = faceIdList->size();
}