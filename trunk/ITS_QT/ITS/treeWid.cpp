#include "treeWid.h"


treeWid::treeWid(QWidget* prnt,db* database,QTreeWidget* treeWidge,mdiDS* mdids,mdi* mdiArea)
{
	this->database = database;
	this->treeWidget = treeWidge;
	this->mdids = mdids;
	this->mdiArea = mdiArea;
	itemList = new QList<QTreeWidgetItem*>();

	buildTree();

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

void treeWid::selectedItemChange(){
	QString selection(treeWidget->currentItem()->text(0));
	QString personName;
	if(selection == "Photos" || selection == "Faces"){
		personName = treeWidget->currentItem()->parent()->text(0);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if(selection == "Photos" ){
			QList<photo*>* photoList = database->selectPersonPhoto(personName);
			displayPhoto(photoList);	
		}else{
			QList<face*>* faceList = database->selectPersonFace(personName);
			displayFace(faceList);
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

void treeWid::displayPhoto(QList<photo*>* photoList){
	mdids->clearMdiDS();
	for(int i = 0; i< photoList->size(); i++){
		mdids->addMdiDSChild((*photoList)[i]);
	}
}

void treeWid::displayFace(QList<face*>* faceList){
	mdids->clearMdiDS();
	for(int i = 0; i< faceList->size(); i++){
		mdiArea->addMdiFace((*faceList)[i]);
	}
}