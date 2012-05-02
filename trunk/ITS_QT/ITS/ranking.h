#ifndef RANKING_H
#define RANKING_H

#include "face.h"
#include "Rank.h"
#include "db.h"


class ranking
{
public:
	ranking(db*);
	~ranking(void);
	db* dbObj;
	double compareFaces(face* , face*);
	void rankCompare(int);
	void kNN();
	QList<int>* getUnknownFaceIDs();
	QList<int>* getfaceIDsFromLabelId(int );
	double getSimilarity(int , int );
	int findDominantId(Rank*,int,int);
	char* getLabelString(int);

	int bordoranking(int unknownFaceId);
	double getRank(int labelId, int unknownId);
	double getCompValue(QList<Rank*> rank,int size,int id);
	int getPosition(QList<Rank*> ranks, int size, int controlId);
	
};

#endif