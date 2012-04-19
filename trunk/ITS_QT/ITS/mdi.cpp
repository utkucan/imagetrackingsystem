#include "mdi.h"


mdi::mdi(QMdiArea* mdiArea)
{
	this->mdiArea = mdiArea;
	childPosx = 0;
	childPosy = 0;
}


mdi::~mdi(void)
{
}

void mdi::addMdiPhoto(photo* photoObject){
	if(photoWindows.size()>0){
		photoWindows[0]->close();
		delete photoWindows[0];
		photoWindows.removeFirst();
		mdiArea->closeAllSubWindows();
//		return;
	}
	mdiPhoto *child = new mdiPhoto(mdiArea,photoObject);
	
	photoWindows.append(child);
	child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));
	 child->setChildGeometry(0,0);
	// child'lar� array'e koy, arraydaki pozisyon�ar�na g�re, pas�lacaklar� yeri ayarla panpa
	// ekrana bast���n y�zleri ayn� zamanda bir file'a kaydet, tekrar i�lem yapmak zorunda kalma
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
	// child'lar� array'e koy, arraydaki pozisyon�ar�na g�re, pas�lacaklar� yeri ayarla panpa
	// ekrana bast���n y�zleri ayn� zamanda bir file'a kaydet, tekrar i�lem yapmak zorunda kalma
	// childPosx = (childPosx+130)%650;
	// if(childPosx == 0)
	//	childPosy += 130;
    child->show();
}
*/
void mdi::addMdiFace(face* faceObject){

	mdiFace *child = new mdiFace(mdiArea,faceObject);
	
	faceWindows.append(child);
	child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));
	child->setChildGeometry(childPosx+5,childPosy+5);
	// child'lar� array'e koy, arraydaki pozisyon�ar�na g�re, pas�lacaklar� yeri ayarla panpa
	// ekrana bast���n y�zleri ayn� zamanda bir file'a kaydet, tekrar i�lem yapmak zorunda kalma
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