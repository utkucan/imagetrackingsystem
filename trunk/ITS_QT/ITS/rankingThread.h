#ifndef RANKINGTHREAD_H
#define RANKINGTHREAD_H
#include <QThread>
#include <QList>
#include "ranking.h"
#include "face.h"
#include "db.h"
class rankingThread :  public QThread
{
public:
	rankingThread();
	~rankingThread(void);
	void initRanking(db* database,QList<face*>* faceList);
	void run();
private:
	ranking* r;
};
 #endif