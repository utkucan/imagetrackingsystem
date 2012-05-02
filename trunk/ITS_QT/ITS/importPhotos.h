#ifndef IMPORTPHOTOS_H
#define IMPORTPHOTOS_H

#include <QThread>
#include <QStringList>
#include <QString>
#include <QMutex>

#include <string>
#include <iostream>
#include <fstream>

#include "mdiDS.h"
#include "photo.h"
#include "face.h"
#include "analyzer.h"
#include "db.h"
#include "ranking.h"
#include "matlab.h"

class importPhotos  : public QThread
{
public:
	importPhotos(QString,db*,QList<photo*> *photos,QList<face*> *faces,matlab* m);
	importPhotos(QStringList,db*,QList<photo*> *photos,QList<face*> *faces,matlab* m);
	~importPhotos(void);
	
	void run();
	void lockProcess(){
		m1->lock();
	}
	void wakeProcess(){
		m1->unlock();
	}
private:
	void findImage(QString inp);
	string QStringToString(QString);

	bool flag;
	QMutex* m1;
	QStringList* QDirectory;
	QString dir;
	QList<photo*> *photos;
	QList<face*> *faces;
	int maxNumOfThread;
	db* database;
	matlab* m;
};
#endif
