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
#include "ranking.h"
#include "rankingThread.h"
class importPhotos  : public QThread
{
public:
	importPhotos(QString,db*,QList<photo*> *photos,QList<face*> *faces,matlab* m,rankingThread* rt);
	importPhotos(QStringList,db*,QList<photo*> *photos,QList<face*> *faces,matlab* m,rankingThread* rt);
	~importPhotos(void);
	
	void run();
	void lockProcess(){
		m1->lock();
	}
	void wakeProcess(){
		m1->unlock();
	}
	void setStop(bool flag){stopFlag = flag;}
private:
	void findImage(QString inp);
	string QStringToString(QString);
	bool stopFlag;
	bool flag;
	QMutex* m1;
	QStringList* QDirectory;
	QString dir;
	QList<photo*> *photos;
	QList<face*> *faces;
	int maxNumOfThread;
	db* database;
	matlab* m;
	rankingThread* rt;
};
#endif
