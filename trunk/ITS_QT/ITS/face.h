#ifndef FACE_H
#define FACE_H
#include <QList>
#include <QImage>
#include <string>
#include <iostream>
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
		QImage* faceImage,
		QList<double> *featureList,
		string label);
	~face(void);
	int getID();
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	QList<double>* getFecialFeatures();
	QImage* getImage();
	string getPath();
	string getLabel();
	void setLavel(string label);
	int getTransformedWidth(){return tw;}
	int getTransformedHeight(){return th;}
	void setID(int id){ this->id = id;}
	void setLabel(string lbl){label = lbl;}
private:

	int id;
	int x,y,width,height,tw,th;
	string imagePath;
	string label;
	QList<double> *fecialFeatureList;
	QImage* faceImage;
};
#endif
