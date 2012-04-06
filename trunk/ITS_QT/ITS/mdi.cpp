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

void mdi::addMdiChild(face* faceObject){

	MdiChild *child = new MdiChild(mdiArea,faceObject);
	
	childWindows.append(child);
	child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));
	child->setChildGeometry(childPosx+5,childPosy+5);
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