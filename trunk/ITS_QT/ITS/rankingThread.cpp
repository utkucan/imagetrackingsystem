#include "rankingThread.h"


rankingThread::rankingThread()
{
	
}


rankingThread::~rankingThread(void)
{
}

void rankingThread::initRanking(db* database,QList<face*>* faceList){
	r = new ranking(database,faceList);
}

void rankingThread::run(){
	if(r->isRunning())
		r->setStop(true);
	while(r->isRunning());
	r->setStop(false);
	r->start();
}
