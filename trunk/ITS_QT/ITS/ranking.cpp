#include "ranking.h"



ranking::ranking(db* d)
{
	dbObj = d;
}


ranking::~ranking(void)
{
}


double ranking::compareFaces(face* f1 , face* f2){
	double* a1 = f1->getFecialFeatures();
	double* a2 = f2->getFecialFeatures();
	if(f1->getID()== f2->getID())
		return 0;

	double sum = 0;
	for(int i = 0 ; i < 13 ; i++){
		double sSum = 0;
		for(int j = 0 ; j < 149 ; j++)
		{		
			sSum += (a1[149*i+j]-a2[149*i+j])*(a1[149*i+j]-a2[149*i+j]);
		}
		sum += sqrt(sSum);
	}

	if(f1->getID() >f2->getID()){
		dbObj->insertIntoEqual(f1->getID(),f2->getID(),sum/13);
	}else{
		dbObj->insertIntoEqual(f2->getID(),f1->getID(),sum/13);
	}

	return sum/13;
}

void ranking::rankCompare(int labelID){

	//--- bu labelIDye ait diðer resimleri çek 4 tane faceID
	// her resim için bilinmeyen resimlerle karþýlaþtýrma yap
	//--- labelID ye ait faceID çek
	// bütün bilinmeyenlerle karþýlaþtýr
	// getSimilarity(labeledFaceID, unknownFaceId)

}
face* getFaceList(){face* fl;return fl;}

int getNumOfFaces(){
	return 0;
}

void ranking::kNN(){
	QList<int>* unknownFaceList = getUnknownFaceIDs();	// bütün labelsýz yüzlerin idlerini aldýk
	face* faceList = getFaceList(); // bütün yüzleri aldýk
		
	int faceNum = getNumOfFaces(); // toplam yüz sayýsý
	
	for(int k = 0 ; k<unknownFaceList->size(); k++){ // her bilinmeyen yüz için
		Rank* rankList = new Rank[getNumOfFaces()]; // rank listesi oluþtur
		for(int i = 0 ; i<faceNum;i++){ // bütün yüzlerle karþýlaþtýr
			rankList[i].similarityRate= getSimilarity(faceList[i].getID() , unknownFaceList->at(k)); 	
			rankList[i].faceID = faceList[i].getID(); 	
		}
		faceList[k].setLabel(getLabelString(findDominantId(rankList,getNumOfFaces(),5))); 
	}

}

char* ranking::getLabelString(int id){return "";}

int compare(Rank* a, Rank* b){
	if ( a->similarityRate < b->similarityRate)
      return -1;
   else if (a->similarityRate > b->similarityRate)
      return 1;
   else
      return 0;
}

typedef int (*compfn)(const void*, const void*);

int ranking::findDominantId(Rank* rl,int size, int range){
	
	qsort(rl, size, sizeof(struct Rank), (compfn)compare);

	int dominantLabel = 0;
	int dominant = 0;
	int count;
	for(int i = 0 ; i<range; i++){

		dominantLabel = rl[i].faceID;
		count = 0;

		for(int k = 0 ; k < range ; k++) {
			if(rl[k].faceID == dominantLabel)
				count++;
		}
		if(dominant<count){
			dominant = count;
			dominantLabel = rl[i].similarityRate;
		}
	}
	if(dominant>(range/2))
		return dominant;
	else
		return -1;
}

QList<int>* ranking::getUnknownFaceIDs(){
	QList<int>* ufl;

// gets list of faceIDs which do not have LABEL

	return ufl =dbObj->getAllUnknownFaceIDs() ;
}
QList<int>*  ranking::getfaceIDsFromLabelId(int labeledFaceId){
QList<int>*  r;

// gets list of faceIds from labelID

return r;
}


double ranking::getSimilarity(int faceID1 , int faceID2){
	double r =0;
	
	// get similarity value between two faces

	return r;
}

int ranking::bordoranking(int unknownFaceId){ //en cok benzeyen kisinin labelIdsini don
	int* labels; //butun labelId'ler
	int labelsLength;
	int minIndex = 0;
	double min = getRank(labels[0],unknownFaceId);
	for(int i = 1 ; i < labelsLength ; i++){
		double rnk = getRank(labels[i],unknownFaceId);
		if(rnk < min){
			min = rnk;
			minIndex = i;
		}
	}
	return labels[minIndex];
	
}
double ranking::getRank(int labelId, int unknownId){
	QList<int>*  faceId = getfaceIDsFromLabelId(labelId); //labelId si labelId olan tum faceId'ler

	double pos = 0;
	for(int i = 0 ; i < faceId->size() ; i++){
		QList<Rank*> ranks = dbObj->selectFromEqual(faceId->at(i)); // similarity tablosunda bir faceIdsi faceId[i] olan tum faceIdler ve similiratileri 
		int ranksLength  = ranks.size();
		pos = pos + getPosition(ranks,ranksLength , unknownId);
	}
	return pos/faceId->size();
}

double ranking::getCompValue(QList<Rank*> rank,int size,int id){
	for(int i = 0 ; i < size ; i++)
	{
		if(rank.at(i)->faceID == id)
			return rank.at(i)->similarityRate;
	}
}

int ranking::getPosition(QList<Rank*> ranks, int size, int controlId){
	int ctr = 0;
	double value = getCompValue(ranks,-size,controlId);
	for(int i = 0 ; i < size ; i++)
	{
		if(ranks.at(i)->similarityRate <= value)
			ctr++;
	}
	return ctr;
}