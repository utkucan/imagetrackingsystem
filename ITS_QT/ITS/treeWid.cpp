#include "treeWid.h"


treeWid::treeWid(QWidget* prnt,db* database,QTreeWidget* treeWidge,mdiDS* mdids,mdi* mdiArea)
{

	personName = "";
	this->database = database;
	treeWidge->setFont(QFont("Fantasy", 10, QFont::Bold));
	this->treeWidget = treeWidge;
	this->mdids = mdids;
	this->mdiArea = mdiArea;

	SearchItemList = new QList<QTreeWidgetItem*>();
	lastOperation = -1;
	listPos = 0;
	reDoFlag = false;
	reDoRunning = false;
	updateFaces = false;
	personName = "";

	photoList = new QList<photo*>();
	faceList = new QList<face*>();

	QStringList personList = database->getAllPerson();
	personPosList = personList;
	buildSearchTree(personList);
	buildTree(personList);
	buildLists();
	this->mdiArea->setFaceList(faceList);
	checkRecognizedFaces();

}


treeWid::~treeWid(void)
{
}

void treeWid::checkRecognizedFaces(){
	
	QList<QString>* lbls = new QList<QString>();
	QList<int>* counts = new QList<int>();
	int totalCount = 0;
	database->numOfPersonsRecognizedFace(lbls,counts);

	for(int i = 0; i<personPosList.size();i++){
		if(!lbls->contains(personPosList[i])){
			personNonApprovedFaceCount[i] = 0;
			itemPhotos->child(i)->setText(0,QString("%1").arg(personPosList[i]));
		}else{
			int pos = lbls->indexOf(personPosList[i]);
			totalCount += (*counts)[pos];
			if(personNonApprovedFaceCount[i] != (*counts)[pos]){
				if(personName != personPosList[i]){
					//itemPhotos->child(i)->setTextColor(0,QColor(255,0,0,255));
					itemPhotos->child(i)->setText(0,QString("%1 (%2)").arg(personPosList[i]).arg((*counts)[pos]));
				}else{
					if(lastOperation == operation::faceOperation){
						reDoRunning = true;
			//			QApplication::setOverrideCursor(Qt::WaitCursor);
						reDoLastOperation();
						int c = mdiArea->getFaceCount();
						if(c!=0)
							itemPhotos->child(i)->setText(0,QString("%1 (%2)").arg(personPosList[i]).arg(mdiArea->getFaceCount()));
						else
							itemPhotos->child(i)->setText(0,QString("%1").arg(personPosList[i]));
			//			QApplication::restoreOverrideCursor();
					}else{
						itemPhotos->child(i)->setText(0,QString("%1 (%2)").arg(personPosList[i]).arg((*counts)[pos]));
					}
				}
				personNonApprovedFaceCount[i] = (*counts)[pos];
			}
		}
	}
	if(personNonApprovedFaceCount[0] != totalCount){	
		personNonApprovedFaceCount[0] = totalCount;
		if(personName != personPosList[0]){
	//		itemPhotos->child(0)->setTextColor(0,QColor(255,0,0,255));
		}
		if(totalCount != 0)
			itemPhotos->child(0)->setText(0,QString("%1 (%2)").arg(personPosList[0]).arg(totalCount));
		else
			itemPhotos->child(0)->setText(0,QString("%1").arg(personPosList[0]));
	}

	counts->clear();
	delete counts;
	lbls->clear();
	delete lbls;
	
}

void treeWid::updateTree(){
	QStringList personList = database->getAllPerson();
	if(size < personList.size()){
		for( int i = size; i<personList.size();i++){
			personPosList.append(personList[i]);
			personNonApprovedFaceCount.append(0);
			QTreeWidgetItem* SearchItem = new QTreeWidgetItem(itemSearch);
			SearchItem->setText(0,personList[i]);
			SearchItem->setCheckState(0,Qt::Unchecked);
			SearchItemList->append(SearchItem);

			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0,personList[i]);
			
			

			QTreeWidgetItem *subPhotoItem = new QTreeWidgetItem();
			subPhotoItem->setText(0,"Photos");
			item->addChild(subPhotoItem);

			QTreeWidgetItem *subFaceItem = new QTreeWidgetItem();
			subFaceItem->setText(0,"Faces");
			item->addChild(subFaceItem);

			itemPhotos->addChild(item->clone());
			
//			itemList->append(item);
		}
		size = personList.size();
	}else if(size > personList.size()){
		
		for(int i = 0; i<personPosList.size();i++){
			if(!personList.contains(personPosList[i])){
				int a = itemSearch->childCount();
				itemSearch->removeChild(itemSearch->child(i-1));
				int b = itemSearch->childCount();
				itemPhotos->removeChild(itemPhotos->child(i));
				personPosList.removeAt(i);
				personNonApprovedFaceCount.removeAt(i);
				i--;
			}
		}
//		treeWidget->update();
		size = personList.size();
	}
//	checkRecognizedFaces();
}

void treeWid::buildTree(QStringList personList){

	
	itemPhotos = new QTreeWidgetItem(treeWidget);
	itemPhotos->setText(0,"People");

	
	size = personList.size();
	for( int i = 0; i<personList.size();i++){
		personNonApprovedFaceCount.append(0);
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
	personName = "";
	if(selection == "Photos" || selection == "Faces"){
		reDoFlag = false;
		/*
		if(personName != treeWidget->currentItem()->parent()->text(0))
			mdids->clearMdiDS();
		*/
		personName = treeWidget->currentItem()->parent()->text(0);
		int p = personName.indexOf(" (");
		if(p>0)
			personName = personName.remove(p,personName.length()-p);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if(selection == "Photos" ){
			lastOperation = operation::photoOperation;
			QList<int>* photoIDList = database->selectPersonPhoto(personName);
			displayPhoto(photoIDList);	
			photoIDList->clear();
			delete photoIDList;
		}else{
			int pos = personPosList.indexOf(personName);
	//		itemPhotos->child(pos)->setTextColor(0,QColor(0,0,0,255));
			lastOperation = operation::faceOperation;
			QList<int>*ApprovedList = new QList<int>();
			QList<int>* faceIdList;
			if(personName == "Unknown")
				faceIdList = database->getAllUnknownFaceIDs();
			else
				faceIdList = database->selectPersonFace(personName,ApprovedList);
			displayFace(faceIdList,ApprovedList);
			faceIdList->clear();
			ApprovedList->clear();
			delete faceIdList;
			delete ApprovedList;
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
				photoIDList->clear();
				delete photoIDList;
				
			}else{
				if(treeWidget->currentItem()->isExpanded())
					treeWidget->collapse(treeWidget->currentIndex());
				else
					treeWidget->expand(treeWidget->currentIndex());
			}
		}
	}
}

void treeWid::reDoLastOperation(){
	if(lastOperation != -1){
		reDoFlag = true;
		if(lastOperation == operation::photoOperation){
			QList<int>* photoIdList = database->selectPersonPhoto(personName);
			displayPhoto(photoIdList);
			photoIdList->clear();
			delete photoIdList;
		}else if(lastOperation == operation::faceOperation){
			/*
			int pos = personPosList.indexOf(personName);
			itemPhotos->child(pos)->setTextColor(0,QColor(0,0,0,255));
			*/
			QList<int>*ApprovedList = new QList<int>();
			QList<int>* faceIdList;
			if(personName == "Unknown")
				faceIdList = database->getAllUnknownFaceIDs();
			else
				faceIdList = database->selectPersonFace(personName,ApprovedList);
			displayFace(faceIdList,ApprovedList);
			faceIdList->clear();
			ApprovedList->clear();
			delete faceIdList;
			delete ApprovedList;
		}else if(lastOperation == operation::SearchOperation){
			
			QList<int>* photoIdList = database->selectPersonsInPhoto(SearchPersonList);
			displayPhoto(photoIdList);
			photoIdList->clear();
			delete photoIdList;
		}
		reDoRunning = false;
	}
}

void treeWid::displayPhoto(QList<int>* photoIdList){
	mdiArea->photoDisplayGeomety();
	mdids->setvisible(true);
	updateFaces = true;
	if(!reDoFlag){
		mdids->clearMdiDS();
		listPos = 0;
	}
	for(int i = listPos; i< photoIdList->size(); i++){
		if(i == photoIdList->size()-1)
			updateFaces = false;
		mdids->addMdiDSChild((*photoList)[(*photoIdList)[i]-1]);
	}
	updateFaces = false;
	listPos = photoIdList->size();
}

void treeWid::displayFace(QList<int>* faceIdList,QList<int>*ApprovedList){
	mdiArea->faceDisplayGeomety();
	mdids->setvisible(false);
	updateFaces = true;
	if(!reDoFlag){
		mdids->clearMdiDS();
		listPos = 0;
	}
	else if(reDoRunning){
		QList<int> preDisplayedFaces = mdiArea->displayedFaces();
		for(int i = 0; i<faceIdList->size(); i++){
			if(preDisplayedFaces.contains((*faceIdList)[i])){
				faceIdList->removeAt(i);
				i--;
			}else{
				preDisplayedFaces.append((*faceIdList)[i]);
			}
		}
		listPos = 0;
	}
	/*

	QList<int> preDisplayedFaces = mdiArea->displayedFaces();
	for(int i = 0; i<faceIdList->size(); i++){
		if(preDisplayedFaces.contains((*faceIdList)[i])){
			faceIdList->removeAt(i);
			i--;
		}else{
			preDisplayedFaces.append((*faceIdList)[i]);
		}
	}
	listPos = 0;
	*/
	for(int i = listPos; i< faceIdList->size(); i++){
		if(i == faceIdList->size()-1)
			updateFaces = false;
		if(personName == "Unknown"){
			face* fc = (*faceList)[(*faceIdList)[i]-1];
			mdiArea->addMdiFace(fc,0);
		}else if((*ApprovedList)[i] == 0){
			face* fc = (*faceList)[(*faceIdList)[i]-1];
			mdiArea->addMdiFace(fc,1);
		}
	}
	updateFaces = false;
	listPos = faceIdList->size();
}

string treeWid::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}