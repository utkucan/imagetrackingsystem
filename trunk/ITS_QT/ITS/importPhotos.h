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

class importPhotos  : public QThread
{
public:
	importPhotos(QString,db*,QList<photo*> *photos,QList<face*> *faces);
	importPhotos(QStringList,db*,QList<photo*> *photos,QList<face*> *faces);
	~importPhotos(void);
	
	void run();
	void lockProcess(){
		m->lock();
	}
	void wakeProcess(){
		m->unlock();
	}
private:
	void findImage(QString inp);
	string QStringToString(QString);

	bool flag;
	QMutex* m;
	QStringList* QDirectory;
	QString dir;
	QList<photo*> *photos;
	QList<face*> *faces;
	int maxNumOfThread;
	db* database;
};
#endif
