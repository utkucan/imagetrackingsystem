#include "treeWid.h"


treeWid::treeWid(QWidget* prnt,db* database,QTreeWidget* treeWidge,mdiDS* mdids,mdi* mdiArea)
{
	this->database = database;
	this->treeWidget = treeWidge;
	this->mdids = mdids;
	this->mdiArea = mdiArea;
	FilterItemList = new QList<QTreeWidgetItem*>();
	lastOperation = -1;
	listPos = 0;
	reDoFlag = false;

	photoList = new QList<photo*>();
	faceList = new QList<face*>();

	QStringList personList = database->getAllPerson();
	buildFilterTree(personList);
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
			QTreeWidgetItem* FilterItem = new QTreeWidgetItem(itemFilter);
			FilterItem->setText(0,personList[i]);
			FilterItem->setCheckState(0,Qt::Unchecked);
			FilterItemList->append(FilterItem);

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
	itemPhotos->setText(0,"Photos");

	
	size = personList.size();
	for( int i = 0; i<personList.size();i++){
		QTreeWidgetItem* item = new QTreeWidgetItem(/*treeWidget*/);
		item->setText(0,personList[i]);
		//item->setCheckState(0,Qt::Unchecked);

		QTreeWidgetItem *subPhotoItem = new QTreeWidgetItem();
		subPhotoItem->setText(0,"Labeled Photos");
		item->addChild(subPhotoItem);

		QTreeWidgetItem *subFaceItem = new QTreeWidgetItem();
		subFaceItem->setText(0,"Faces");
		item->addChild(subFaceItem);

		itemPhotos->addChild(item->clone());

//		itemList->append(item);
	}

	

}

void treeWid::buildFilterTree(QStringList personList){
	itemFilter = new QTreeWidgetItem(treeWidget);
	itemFilter->setText(0,"Filter");

	for( int i = 1; i<personList.size();i++){
		QTreeWidgetItem* item = new QTreeWidgetItem(itemFilter);
		item->setCheckState(0,Qt::Unchecked);
		item->setText(0,personList[i]);
		FilterItemList->append(item);
	}

}

void treeWid::buildLists(){
	database->getAllPhotos(photoList,faceList);
}

void treeWid::selectedItemChange(){
	QString selection(treeWidget->currentItem()->text(0));
	if(selection == "Labeled Photos" || selection == "Faces"){
		reDoFlag = false;
		personName = treeWidget->currentItem()->parent()->text(0);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if(selection == "Labeled Photos" ){
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
		if(treeWidget->currentItem()->text(0) == "Photos" || treeWidget->currentItem()->text(0) == "Filter"){
			if(treeWidget->currentItem()->isExpanded())
				treeWidget->collapse(treeWidget->currentIndex());
			else
				treeWidget->expand(treeWidget->currentIndex());
			for(int i = 0; i< FilterItemList->size(); i++){
				(*FilterItemList)[i]->setCheckState(0,Qt::Unchecked);
			}
		}else{
			if(treeWidget->currentItem()->parent()->text(0) == "Filter"){
				filterPersonList.clear();
				for(int i = 0; i< FilterItemList->size(); i++){
					if((*FilterItemList)[i]->checkState(0) == Qt::Checked)
						filterPersonList.append((*FilterItemList)[i]->text(0));
				}
				int a = 5;
				// databaseden photolarý çek
				// db nin içine selectPersonsPhoto diye bir method yaz, QStringList alsýn
				// listenin içindeki labellarýn ortak fotolarýný bulsun gelsin felan
				/*
				lastOperation = operation::filterOperation;
				QList<int>* photoIDList = database->selectPersonsPhoto(filterPersonList);
				displayPhoto(photoIDList);	
				*/
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
		}else if(lastOperation == operation::filterOperation){
			/*
			QList<int>* photoIdList = database->selectPersonsPhoto(filterPersonList);
			displayPhoto(photoIdList);
			*/
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

void treeWid::displayFace(QList<int>* faceIdList,QList<int>*ApprovedList){
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