#include "face.h"

face::face(int id,string imagePath,int x,int y,int width,int height,int transformedWidth,int transformedHeight/*, QImage* faceImage*/,double *featureList,string label)
{
	this->id = id;
	this->imagePath = imagePath;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->tw = transformedWidth;
	this->th = transformedHeight;
	this->fecialFeatureList = featureList;
	faceImage = NULL;
	loadFaceImage();
//	this->faceImage = faceImage;
	this->label = label;
}

face::~face(void)
{
	//fecialFeatureList->clear();
	if(fecialFeatureList != NULL){
		delete fecialFeatureList;
		fecialFeatureList = NULL;
	}

	if(faceImage != NULL){
		delete faceImage;
		faceImage = NULL;
	}
}

void face::loadFaceImage(){
	try{
		IplImage* img = cvLoadImage(imagePath.c_str());
		if(!img){
			//errmsg = "Can not load image " + imagename;
			return;// false;
		}
		IplImage* temp = cvCreateImage( cvSize( width, height), img->depth, img->nChannels );
		cvSetImageROI(img,cvRect( x, y, width, height));
		cvCopy( img, temp ); 
		cvResetImageROI( img );

		faceImage = IplImage2QImage(temp);
		cvReleaseImage(&img);
		cvReleaseImage(&temp);
	}catch(exception e){
		//errmsg = e.what();
		return;// false;
	}
}

int face::getID(){return id;}

int face::getX(){return x;}

int face::getY(){return y;}

int face::getWidth(){return width;}

int face::getHeight(){return height;}

double* face::getFecialFeatures(){return fecialFeatureList;}

QImage* face::getImage(){
	/*if(faceImage == NULL)
		loadFaceImage();*/
	return faceImage;
}

string face::getPath(){return imagePath;}

string face::getLabel(){return label;}

void face::setLavel(string label){this->label = label;}

QImage* face::IplImage2QImage(const IplImage *iplImage)
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
//		qWarning() << "Image cannot be converted.";
		return new QImage();
	}
}
