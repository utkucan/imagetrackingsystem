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
	void setFaceId(int unknownFaceId){this->unknownFaceId =unknownFaceId;}
	double compareFaces(face* , face*);
	void rankCompare(int);
	void kNN();
	QList<int>* getUnknownFaceIDs(QList<face*>*);
	QList<int>* getfaceIDsFromLabelId(int );

	char* getLabelString(int);

	double getSimilarity(int , int );
	int findDominantId(Rank*,int,int);

	int bordoranking(int unknownFaceId);
	double getRank(int labelId, int unknownId);
	double getCompValue(QList<Rank*> rank,int size,int id);
	int getPosition(QList<Rank*> ranks, int size, int controlId);
	QList<face*> *faceList;
	string QStringToString(QString);
	int unknownFaceId;
};

#endif