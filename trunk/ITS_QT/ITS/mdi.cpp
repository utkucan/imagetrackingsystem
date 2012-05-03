#include "mdi.h"


mdi::mdi(QMdiArea* mdiArea,db* database)
{
	this->mdiArea = mdiArea;
	this->database = database;
	childPosx = 0;
	childPosy = 0;
}


mdi::~mdi(void)
{
}

void mdi::updateFace(){
	int cursize = faceWindows.size();
	QList<mdiFace*> tmpList;
	for(int i = 0; i<faceWindows.size();i++){
		if(!faceWindows[i]->isClosed()){
			tmpList.append(faceWindows[i]);
	/*		faceWindows[i]->setChildGeometry(childPosx+5,childPosy+5);
			childPosx = (childPosx+130)%650;
			if(childPosx == 0)
				childPosy += 130;
				*/
		}
	}

	if(cursize != tmpList.size()){
		childPosx = 0;
		childPosy = 0;
		QScrollBar* scroll = mdiArea->verticalScrollBar();
		int cury = scroll->value();
		scroll->setValue(0);
		for(int i = 0; i<tmpList.size();i++){
				
				int y = scroll->value();
				int cpy = childPosy - y;
				tmpList[i]->setChildGeometry(childPosx+5,cpy+5);
			//tmpList[i]->setChildGeometry(childPosx+5,childPosy+5);
			childPosx = (childPosx+130)%650;
			if(childPosx == 0)
				childPosy += 130;
		}
		scroll->setValue(cury);
		faceWindows.clear();
		faceWindows = tmpList;
	}else
		tmpList.clear();

	
	/*
	int cursize= faceWindows.size();
	for(int i = 0; i<faceWindows.size();i++){
		if(faceWindows[i]->isClosed()){
			delete faceWindows[i];
			faceWindows.removeAt(i);
			i--;
		}
	}
	if(cursize>faceWindows.size()){
		childPosx = 0;
		childPosy = 0;
		for(int i = 0; i<faceWindows.size();i++){
			faceWindows[i]->setChildGeometry(childPosx+5,childPosy+5);
			childPosx = (childPosx+130)%650;
			if(childPosx == 0)
				childPosy += 130;
		}
	}
	*/
 //   child->show();
}

void mdi::clearPhotos(){
	photoWindows.clear();
	faceWindows.clear();

	/*
	while(photoWindows.size()>0){
		photoWindows[0]->close();
		delete photoWindows[0];
		photoWindows.removeFirst();
	}

	while(faceWindows.size()>0){
		faceWindows[0]->close();
		delete photoWindows[0];
		photoWindows.removeFirst();
	}
	*/

	childPosx = 0;
	childPosy = 0;
	mdiArea->closeAllSubWindows();
}

void mdi::reDisplayLastPhoto(){
	if(photoWindows.size()>0 ){
		if(photoWindows[0]->isClosed()){
			delete photoWindows[0];
			photoWindows.removeFirst();
			mdiPhoto *child = new mdiPhoto(mdiArea,photolst[0],database);
	
			photoWindows.append(child);
			child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));
			child->setChildGeometry(0,0);
			child->show();
		}
	}

}

void mdi::addMdiPhoto(photo* photoObject){
	if(photoWindows.size()>0){
		photoWindows[0]->close();
		delete photoWindows[0];
		photoWindows.removeFirst();
		mdiArea->closeAllSubWindows();
		photolst[0] = NULL;
		photolst.clear();
//		return;
	}
	mdiPhoto *child = new mdiPhoto(mdiArea,photoObject,database);
	
	photoWindows.append(child);
	photolst.append(photoObject);
	child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));
	 child->setChildGeometry(0,0);
	// child'larý array'e koy, arraydaki pozisyonþarýna göre, pasýlacaklarý yeri ayarla panpa
	// ekrana bastýðýn yüzleri ayný zamanda bir file'a kaydet, tekrar iþlem yapmak zorunda kalma
	// childPosx = (childPosx+130)%650;
	// if(childPosx == 0)
	//	childPosy += 130;
    child->show();
}

/*
void mdi::addMdiPhoto(photo* photoObject){
	mdiPhoto *child = new mdiPhoto(mdiArea,photoObject);
	
	photoWindows.append(child);
	child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));
	 child->setChildGeometry(childPosx+5,childPosy+5);
	// child'larý array'e koy, arraydaki pozisyonþarýna göre, pasýlacaklarý yeri ayarla panpa
	// ekrana bastýðýn yüzleri ayný zamanda bir file'a kaydet, tekrar iþlem yapmak zorunda kalma
	// childPosx = (childPosx+130)%650;
	// if(childPosx == 0)
	//	childPosy += 130;
    child->show();
}
*/
void mdi::addMdiFace(face* faceObject){

	mdiFace *child = new mdiFace(mdiArea,faceObject,database);

	faceWindows.append(child);
	child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));

	QScrollBar* scroll = mdiArea->verticalScrollBar();
	int y = scroll->value();
	int cpy = childPosy - y;
	child->setChildGeometry(childPosx+5,cpy+5);
	// child'larý array'e koy, arraydaki pozisyonþarýna göre, pasýlacaklarý yeri ayarla panpa
	// ekrana bastýðýn yüzleri ayný zamanda bir file'a kaydet, tekrar iþlem yapmak zorunda kalma
	childPosx = (childPosx+130)%650;
	if(childPosx == 0)
		childPosy += 130;
	
    child->show();

	


//	child->showNormal();
//	child->showMaximized();
//	child->showMinimized();
}

QImage* mdi::IplImage2QImage(const IplImage *iplImage)
{
	int height = iplImage->height;
	int width = iplImage->width;
 
	if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
	{
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage* img = new QImage(qImageBuffer, width, height, QImage::Format_RGB888);
		img = new QImage(img->rgbSwapped());
		return img;
	} else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage* img = new QImage(qImageBuffer, width, height, QImage::Format_Indexed8);
 
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++){
			colorTable.push_back(qRgb(i, i, i));
		}
		img->setColorTable(colorTable);
		return img;
	}else{
		qWarning() << "Image cannot be converted.";
		return new QImage();
	}
}

void mdi::setPosition(){
	for( int i = 0; i< photoWindows.size(); i++){
		photoWindows[i]->setPosition();
	}

}

void mdi::faceDisplayGeomety(){
	QRect r = mdiArea->geometry();
	mdiArea->setGeometry(190,10,700,510);
	QRect r1 = mdiArea->geometry();
	int a = 5;
}
void mdi::photoDisplayGeomety(){
	QRect r = mdiArea->geometry();
	mdiArea->setGeometry(190,10,700,400);
	QRect r1 = mdiArea->geometry();
	int a = 5;
}