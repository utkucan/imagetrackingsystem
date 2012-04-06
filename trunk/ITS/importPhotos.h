#ifndef IMPORTPHOTOS_H
#define IMPORTPHOTOS_H

#include <QThread>
#include <QStringList>
#include <QString>

#include <string>
#include <iostream>
#include <fstream>

#include "photo.h"
#include "face.h"
#include "analyzer.h"

class importPhotos  : public QThread
{
public:
	importPhotos(QStringList,QList<photo*>*);
	~importPhotos(void);
	void run();
private:

	string QStringToString(QString);

	QStringList* QDirectory;
	QList<photo*> *photos;
	int maxNumOfThread;
};
#endif
