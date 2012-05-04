#ifndef RANKING_H
#define RANKING_H

#include "face.h"
#include "Rank.h"
#include "db.h"
#include <QThread>
#include <QList>

class ranking :  public QThread
{
public:
	ranking(db*,QList<face*> *faceList);
	~ranking(void);
	db* dbObj;
	void run();
	double compareFaces(face* , face*);
	void rankCompare(int);
	void kNN(QList<face*>*);
	QList<int>* getUnknownFaceIDs(QList<face*>*);
	QList<int>* getfaceIDsFromLabelId(int );
	double getSimilarity(int , int );
	int findDominantId(QList<Rank*>,int,int);
	char* getLabelString(int);

	int bordoranking(int unknownFaceId);
	double getRank(int labelId, int unknownId);
	double getCompValue(QList<Rank*> rank,int size,int id);
	int getPosition(QList<Rank*> ranks, int size, int controlId);
	QList<face*> *faceList;
	string QStringToString(QString);
};

#endif