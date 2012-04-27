#ifndef FACE_H
#define FACE_H
#include <QList>
#include <QImage>
#include <string>
#include <iostream>
#include "facedetect.h"
using namespace std;

class face
{
public:
	face(int id,
		string imagePath,
		int x,
		int y,
		int width,
		int height,
		int transformedWidth,
		int transformedHeight,
//		QImage* faceImage,
		//QList<double> *featureList,
		double* featureList,
		string label);
	~face(void);

	int getID();
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	double* getFecialFeatures();
	QImage* getImage();
	string getPath();
	string getLabel();
	int getTransformedWidth(){return tw;}
	int getTransformedHeight(){return th;}
	int getPhotoId(){return photoId;}

	void setImage(QImage* f){faceImage = f;}
	void setPath(string p){imagePath = p;}
	void setLavel(string label);
	void setID(int id){ this->id = id;}
	void setLabel(string lbl){label = lbl;}
	void setPhotoID(int pid){photoId = pid;}
	
private:
	void loadFaceImage();
	QImage* IplImage2QImage(const IplImage *iplImage);

	int id;
	int photoId;
	int x,y,width,height,tw,th;
	string imagePath;
	string label;
	//QList<double> *fecialFeatureList;
	double* fecialFeatureList;
	QImage* faceImage;
};
#endif
