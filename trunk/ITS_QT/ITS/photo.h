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
	photo(QString,QList<face*>*);
	~photo(void);
	QList<face*>* getFaces();
private:
	string QStringToString(QString);

	string path;
	QList<face*> *faceList;
};
#endif

