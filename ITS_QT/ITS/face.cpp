#include "face.h"

face::face(int id,string imagePath,int x,int y,int width,int height,int transformedWidth,int transformedHeight, QImage* faceImage,QList<double> *featureList,string label)
{
	this->imagePath = imagePath;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->tw = transformedWidth;
	this->th = transformedHeight;
	this->fecialFeatureList = featureList;
	this->faceImage = faceImage;
	this->label = label;
}

face::~face(void)
{
	fecialFeatureList->clear();
	delete fecialFeatureList;
	fecialFeatureList = NULL;
	delete faceImage;
	faceImage = NULL;
}

int face::getID(){return id;}

int face::getX(){return x;}

int face::getY(){return y;}

int face::getWidth(){return width;}

int face::getHeight(){return height;}

QList<double>* face::getFecialFeatures(){return fecialFeatureList;}

QImage* face::getImage(){return (faceImage);}

string face::getPath(){return imagePath;}

string face::getLabel(){return label;}

void face::setLavel(string label){this->label = label;}