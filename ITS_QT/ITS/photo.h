#ifndef PHOTO_H
#define PHOTO_H

#include <QThread>
#include <QString>

#include <string>
#include <iostream>
#include <fstream>

#include "face.h"
class photo
{
public:
	photo(photo*);
	photo(QString,QList<face*>*);
	~photo(void);
	QList<face*>* getFaces();
	string getPath(){return path;}
	QImage* getDSImage(){return downSample;}
	void setID(int id){this->id = id;}
private:
	string QStringToString(QString);
	void downSampleImage();

	int id;
	string path;
	QList<face*> *faceList;
	QImage* downSample;
};
#endif

