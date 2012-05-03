#include "treeWid.h"


treeWid::treeWid(QWidget* prnt,db* database,QTreeWidget* treeWidge,mdiDS* mdids,mdi* mdiArea)
{
	this->database = database;
	this->treeWidget = treeWidge;
	this->mdids = mdids;
	this->mdiArea = mdiArea;
	SearchItemList = new QList<QTreeWidgetItem*>();
	lastOperation = -1;
	listPos = 0;
	reDoFlag = false;

	photoList = new QList<photo*>();
	faceList = new QList<face*>();

	QStringList personList = database->getAllPerson();
	buildSearchTree(personList);
	buildTree(personList);
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
			QTreeWidgetItem* SearchItem = new QTreeWidgetItem(itemSearch);
			SearchItem->setText(0,personList[i]);
			SearchItem->setCheckState(0,Qt::Unchecked);
			SearchItemList->append(SearchItem);

			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0,personList[i]);
			
			

			QTreeWidgetItem *subPhotoItem = new QTreeWidgetItem();
			subPhotoItem->setText(0,"Labeled Photos");
			item->addChild(subPhotoItem);

			QTreeWidgetItem *subFaceItem = new QTreeWidgetItem();
			subFaceItem->setText(0,"Faces");
			item->addChild(subFaceItem);

			itemPhotos->addChild(item->clone());
			
//			itemList->append(item);
		}
		size = personList.size();
	}

}

void treeWid::buildTree(QStringList personList){

	
	itemPhotos = new QTreeWidgetItem(treeWidget);
	itemPhotos->setText(0,"People");

	
	size = personList.size();
	for( int i = 0; i<personList.size();i++){
		QTreeWidgetItem* item = new QTreeWidgetItem(/*treeWidget*/);
		item->setText(0,personList[i]);
		//item->setCheckState(0,Qt::Unchecked);

		QTreeWidgetItem *subPhotoItem = new QTreeWidgetItem();
		subPhotoItem->setText(0,"Photos");
		item->addChild(subPhotoItem);

		QTreeWidgetItem *subFaceItem = new QTreeWidgetItem();
		subFaceItem->setText(0,"Faces");
		item->addChild(subFaceItem);

		itemPhotos->addChild(item->clone());

//		itemList->append(item);
	}

	

}

void treeWid::buildSearchTree(QStringList personList){
	itemSearch = new QTreeWidgetItem(treeWidget);
	itemSearch->setText(0,"Search");

	for( int i = 1; i<personList.size();i++){
		QTreeWidgetItem* item = new QTreeWidgetItem(itemSearch);
		item->setCheckState(0,Qt::Unchecked);
		item->setText(0,personList[i]);
		SearchItemList->append(item);
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
			QList<int>*ApprovedList = new QList<int>();
			QList<int>* faceIdList = database->selectPersonFace(personName,ApprovedList);
			displayFace(faceIdList,ApprovedList);
		}
		QApplication::restoreOverrideCursor();
	}else{
		if(treeWidget->currentItem()->text(0) == "People" || treeWidget->currentItem()->text(0) == "Search"){
			if(treeWidget->currentItem()->isExpanded())
				treeWidget->collapse(treeWidget->currentIndex());
			else
				treeWidget->expand(treeWidget->currentIndex());
			for(int i = 0; i< SearchItemList->size(); i++){
				(*SearchItemList)[i]->setCheckState(0,Qt::Unchecked);
			}
		}else{
			if(treeWidget->currentItem()->parent()->text(0) == "Search"){
				SearchPersonList.clear();
				for(int i = 0; i< SearchItemList->size(); i++){
					if((*SearchItemList)[i]->checkState(0) == Qt::Checked)
						SearchPersonList.append((*SearchItemList)[i]->text(0));
				}
				int a = 5;
				// databaseden photolarý çek
				// db nin içine selectPersonsPhoto diye bir method yaz, QStringList alsýn
				// listenin içindeki labellarýn ortak fotolarýný bulsun gelsin felan
				
				lastOperation = operation::SearchOperation;
				QList<int>* photoIDList = database->selectPersonsInPhoto(SearchPersonList);
				displayPhoto(photoIDList);	
				
			}else{
				if(treeWidget->currentItem()->isExpanded())
					treeWidget->collapse(treeWidget->currentIndex());
				else
					treeWidget->expand(treeWidget->currentIndex());
			}
		}
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
		}else if(lastOperation == operation::faceOperation){
			QList<int>*ApprovedList = new QList<int>();
			QList<int>* faceIdList = database->selectPersonFace(personName,ApprovedList);
			displayFace(faceIdList,ApprovedList);
		}else if(lastOperation == operation::SearchOperation){
			/*
			QList<int>* photoIdList = database->selectPersonsPhoto(SearchPersonList);
			displayPhoto(photoIdList);
			*/
		}

	}
}

void treeWid::displayPhoto(QList<int>* photoIdList){
	mdiArea->photoDisplayGeomety();
	mdids->setvisible(true);
	if(!reDoFlag){
		mdids->clearMdiDS();
		listPos = 0;
	}
	for(int i = listPos; i< photoIdList->size(); i++){
		mdids->addMdiDSChild((*photoList)[(*photoIdList)[i]-1]);
	}
	listPos = photoIdList->size();
}

void treeWid::displayFace(QList<int>* faceIdList,QList<int>*ApprovedList){
	mdiArea->faceDisplayGeomety();
	mdids->setvisible(false);
	if(!reDoFlag){
		mdids->clearMdiDS();
		listPos = 0;
	}
	for(int i = listPos; i< faceIdList->size(); i++){
	//	if((*ApprovedList)[i] == 0)
			mdiArea->addMdiFace((*faceList)[(*faceIdList)[i]-1]);
	}
	listPos = faceIdList->size();
}