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

class importPhotos  : public QThread
{
public:
	importPhotos(QStringList,QList<photo*>*,db*);
	~importPhotos(void);
	void run();
	void lockProcess(){
		m->lock();
	}
	void wakeProcess(){
		m->unlock();
	}
private:

	string QStringToString(QString);
	QMutex* m;
	QStringList* QDirectory;
	QList<photo*> *photos;
	int maxNumOfThread;
	db* database;
};
#endif
