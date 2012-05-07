#ifndef RANKINGTHREAD_H
#define RANKINGTHREAD_H
#include <QThread>
#include <QList>

#include "ranking.h"
#include "db.h"

class rankingThread :  public QThread
{
public:
	rankingThread();
	~rankingThread(void);
	void initRanking(db* database,QList<face*>* faceList);
	void run();
	bool rankingRunning(){return r->isRunning();}
	void stopThread();
	bool getFound(){return r->getFound();}
private:
	ranking* r;
};
 #endif
