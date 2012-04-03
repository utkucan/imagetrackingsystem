#include "analyzer.h"


analyzer::analyzer(QList<face*> *faceList)
{
	this->faceList = faceList;
}


analyzer::~analyzer(void)
{
}

void analyzer::analyz(string imagePath,int idStart){
	facedetect f(imagePath);
	facedetect::List* head = f.getFaceList();
	facedetect::List* tmp = head;
//	int count = 0;

	while( tmp !=NULL){
		QImage* img = IplImage2QImage(tmp->face);

		// burada fecial feature hesaplicaz, NULL yazan yere featurelarý verecez, label'ý þimdilik boþ verdik
		idStart++;
		face* fc = new face(idStart,imagePath,tmp->x,tmp->y,tmp->width,tmp->height,img,NULL,"");
		faceList->append(fc);
//		mdiArea->addMdiChild(fc);
		tmp= tmp->next;
	}
/*
	while( tmp !=NULL){
		QImage* img = IplImage2QImage(tmp->face);

		QPixmap* p = new QPixmap();
		p->convertFromImage(*img,Qt::AutoColor);
		mdiArea->addMdiChild(QString(imagePath.c_str()),p,tmp->x,tmp->y,tmp->width,tmp->height);
		tmp= tmp->next;
	}
	*/
	f.clearList();
}



QImage* analyzer::IplImage2QImage(const IplImage *iplImage)
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