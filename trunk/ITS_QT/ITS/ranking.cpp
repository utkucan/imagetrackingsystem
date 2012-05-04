#include "ranking.h"



ranking::ranking(db* d,QList<face*> *faceList)
{
	dbObj = d;
	this->faceList = faceList;
	close = false;
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

	dbObj->insertIntoEqual(f1->getID(),f2->getID(),sum/13);
	/*
	if(f1->getID() >f2->getID()){
		dbObj->insertIntoEqual(f1->getID(),f2->getID(),sum/13);
	}else{
		dbObj->insertIntoEqual(f2->getID(),f1->getID(),sum/13);
	}
	*/
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


QList<int>* ranking::getUnknownFaceIDs(QList<face*>* fl){
	QList<int>* ufl;
	/*
	for(int k = 0 ; k< fl->size();k++){
		if(fl->at(k)->getLabel().compare("Unknown"))
			ufl->append(fl->at(k)->getID());
	}
	
	*/

// gets list of faceIDs which do not have LABEL

	return ufl = dbObj->getAllUnknownFaceIDs() ;
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

void ranking::run(){
	kNN();

	/*
	if(unknownFaceId != 0){
		int recLblId = bordoranking(unknownFaceId);
		if(recLblId != -1){
			QString lbl = dbObj->getPersonName(recLblId);
			(*faceList)[unknownFaceId-1]->setLabel(QStringToString(lbl));
			dbObj->updateHasFaces(unknownFaceId,lbl,(*faceList)[unknownFaceId-1]->getPhotoId(),0);
		}
	}else{
	
		QList<int> fid = dbObj->getNonApprovedFace();
	
		for(int j = 0 ; j<fid.size(); j++){
			int recLblId = bordoranking(fid[j]);
			if(recLblId != -1){
				QString lbl = dbObj->getPersonName(recLblId);
				(*faceList)[fid[j]-1]->setLabel(QStringToString(lbl));
				dbObj->updateHasFaces(fid[j],lbl,(*faceList)[fid[j]-1]->getPhotoId(),0);
			}
		
		}
	
	}
	*/
}



int ranking::bordoranking(int unknownFaceId){ //en cok benzeyen kisinin labelIdsini don

	QList<int>* persons = dbObj->selectAllPerson(); //butun labelId'ler
	int labelsLength = persons->size();
	int minIndex = 1;
	if(labelsLength<2)
		return -1;
	double min = getRank(persons->at(1),unknownFaceId);
	for(int i = 2 ; i < labelsLength ; i++){
		double rnk = getRank(persons->at(i),unknownFaceId);
		if(rnk < min){
			min = rnk;
			minIndex = i;
		}
	}
	return persons->at(minIndex);
	
}

double ranking::getRank(int labelId, int unknownId){
	QList<int>*  faceId = dbObj->selectAllFacesFromPersonId(labelId); //labelId si labelId olan tum faceId'ler

	double pos = 0;
	for(int i = 0 ; i < faceId->size() ; i++){
		QList<Rank*> ranks = dbObj->selectFaceEqual(faceId->at(i)); // similarity tablosunda bir faceIdsi faceId[i] olan tum faceIdler ve similiratileri 
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
	double value = getCompValue(ranks,size,controlId);
	for(int i = 0 ; i < size ; i++)
	{
		if(ranks.at(i)->similarityRate <= value)
			ctr++;
	}
	return ctr;
}

string ranking::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}


//#include "qdebug.h"

void labelSuggest(QString asd){
//	qDebug()<<"s"<<asd;
}
void ranking::kNN(){
	
	QList<int>* unknownFaceList = dbObj->getAllUnknownFaceIDs();//getUnknownFaceIDs(fl);	// bütün labelsýz yüzlerin idlerini aldýk

	int unknownFaceNum = unknownFaceList->size();

	for(int k = 0 ; k < unknownFaceNum; k++){ // her bilinmeyen yüz için
		if(close)
			break;
		//int x = bordoranking(unknownFaceList->at(k));
		QList<Rank*> rankList = dbObj->selectFromEqual(unknownFaceList->at(k));//= new Rank[fl->size()]; // rank listesi oluþtur
		Rank* rl = new Rank[rankList.size()];
		for(int p = 0 ; p < rankList.size(); p++){
			rl[p].similarityRate = rankList[p]->similarityRate;
			rl[p].faceID = rankList[p]->faceID;
		}
		//QList<int>* persons = dbObj->selectLabelledPerson();
		int size = 0;
		/*
		QList<Rank*> labelledList;
		for(int p = 0 ; p < rankList.size(); p++){
			if(persons->contains(rankList.at(p)->faceID)){
				labelledList.append(rankList.at(p));
			}
		}
		
		for(int p = 0 ; p<labelledList.size();p++){
				rl[p].similarityRate = labelledList.at(p)->similarityRate;
				rl[p].faceID = labelledList.at(p)->faceID;
		}
		*/
		int recLblId = findDominantId(rl, rankList.size(),3);

		int recLblId2 = bordoranking((*unknownFaceList)[k]);
		/*
		if(recLblId != -1){
			QString lbl = dbObj->getPersonName(recLblId);
			(*faceList)[unknownFaceId-1]->setLabel(QStringToString(lbl));
			dbObj->updateHasFaces(unknownFaceId,lbl,(*faceList)[unknownFaceId-1]->getPhotoId(),0);
		}
		*/
		if(recLblId != -1 && recLblId2 != -1 && recLblId2 == recLblId){
			QString lbl = dbObj->getPersonName(recLblId);
			face* fc = (*faceList)[(*unknownFaceList)[k]-1];
			fc->setLabel(QStringToString(lbl));
			dbObj->updateHasFaces(fc->getID(),lbl,fc->getPhotoId(),0);
		}
	
	//	labelSuggest(dbObj->getPersonName(recLblId));//,getLabelString(unknownFaceList->at(k))); //fl->at(k)->
	}
	int a = 5;
		/*
		//for(int i = 0 ; i<faceNum;i++){ // bütün yüzlerle karþýlaþtýr
			//rankList = dbObj->selectFromEqual(unknownFaceList->at(k));// getSimilarity(fl->at(i)->getID(),unknownFaceList->at(k)); 	
			//rankList[i].faceID = fl->at(i)->getID(); 	[i].similarityRate
		//}
		labelSuggest(getLabelString(findDominantId(rankList, fl->size(),5)),getLabelString(unknownFaceList->at(k))); //fl->at(k)->
	*/
}

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
	
	qsort(rl, size, sizeof(Rank), (compfn)compare);//.........................................................................................ben yaptm kanka
	/*
	for(int k = 0 ; k <size ; k++){
		qDebug()<<rl[k].similarityRate<<" "<<k<<" "<<rl[k].faceID;
	}
	*/
	int dominantLabel = 0;
	int dominant = 0;
	int count, dominantId;
	for(int i = 0 ; i<range; i++){

		dominantId = rl[i].faceID;
		count = 0;

		for(int k = 0 ; k < range ; k++) {
			if( rl[k].faceID == dominantId)
				count++;
		}
		if(dominant<count){
			dominant = count;
			dominantLabel = dominantId;
		}
	}
	if(dominant>(int)(range/2)){
	//	qDebug()<<"--"<<dominant<<"--"<<dominantLabel;
		return dominantLabel;
	}else
		return -1;
}