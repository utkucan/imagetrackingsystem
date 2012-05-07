#include "importPhotos.h"

importPhotos::importPhotos(QString dir,db* database,QList<photo*> *photos,QList<face*> *faces,matlab* m,rankingThread* rt){
	this->database = database;
	this->dir = dir;
	this->photos = photos;
	this->faces = faces;
	this->m = m;
	this->rt = rt;
	stopFlag = false;
	QDirectory = new QStringList();
	m1 = new QMutex();
	maxNumOfThread = 1;
	flag = true;
	
}

importPhotos::importPhotos(QStringList QDirectory,db* database,QList<photo*> *photos,QList<face*> *faces,matlab* m,rankingThread* rt)
{
	this->QDirectory = new QStringList(QDirectory);
	this->photos = photos;
	this->database = database;
	this->faces = faces;
	this->m = m;
	this->rt =rt;
	m1 = new QMutex();
	maxNumOfThread = 1;//QDirectory->size();
	flag = false;
}


importPhotos::~importPhotos(void)
{
	
	QDirectory->clear();
	delete QDirectory;
	QDirectory = NULL;
//	photos = NULL;
	
}

void importPhotos::run(){
	QString s = database->GetFirstNonProcessedPhotoPaths();
	if(flag)
		findImage(dir);
	if(QDirectory->size() > 0){
		database->InsertNonProcessedPhotoPaths(*QDirectory);
		QDirectory->clear();
	}
//	QDirectory = new QStringList(database->GetNonProcessedPhotoPaths());
	ranking *r = new ranking(database,faces);
	QList<analyzer*> threads;
	int numOfThread = 0;
//	QList<photo*> *photos;
	//for(int j = 0 ; j< QDirectory->count(); j++){
	QString d = database->GetFirstNonProcessedPhotoPaths();
	while(d != ""){
		if(stopFlag == true)
			break;
		if(!database->photoExist(d)){
			string filename = QStringToString(d);
			QList<face*> *tmpLst = new QList<face*>();

	//		photo* p = new photo((*QDirectory)[j],tmpLst);
	//		photos->append(p);

			analyzer* imageAnalyzer = new analyzer(tmpLst,filename,m);
			imageAnalyzer->start();
			threads.append(imageAnalyzer);
			numOfThread++;

			while(numOfThread == maxNumOfThread){
				for(int i = 0; i< threads.size(); i++){
					if(threads[i]->isFinished()){
					//	QList<face*> *tmpLst = threads[i]->getFaceList();
						if(threads[i]->getFaceList()->size()>0){
							QString path = QString( threads[i]->getFileName().c_str());
							photo* p = new photo( path,threads[i]->getFaceList());
							faces->append(*(threads[i]->getFaceList()));
							photos->append(p);
							QFileInfo* fileInf = new QFileInfo(path);
							QDateTime dt = fileInf->created();
							QString date = dt.toString("dd.MM.yyyy h:m:s ap");
							database->insertIntoPhoto(p,date);
							
							for(int j = 0 ; j<(*(threads[i]->getFaceList())).size(); j++){
								for(int k = 0 ;k<faces->size();k++)
									r->compareFaces((*(threads[i]->getFaceList())).at(j),faces->at(k));
							}

							rt->start();
					//		for(int j = 0 ; j<(*(threads[i]->getFaceList())).size(); j++){
					//			r->setFaceId((*(threads[i]->getFaceList()))[j]->getID());
					//			r->start();
							//	while(!r->isFinished());
								/*
								int recLblId = r->bordoranking((*(threads[i]->getFaceList()))[j]->getID());
								
								if(recLblId != -1){
									QString lbl = database->getPersonName(recLblId);
									(*(threads[i]->getFaceList()))[j]->setLabel(QStringToString(lbl));
									database->updateHasFaces((*(threads[i]->getFaceList()))[j]->getID(),lbl,(*(threads[i]->getFaceList()))[j]->getPhotoId(),0);
								}
								*/
						//	}
							
						}
					//	int pos = photos->size() - threads.size() + i;
					//	database->insertIntoPhoto((*photos)[pos]);

		//				database->DeleteNonProcessedPhotoPaths(QString(threads[i]->getFileName().c_str()));
						numOfThread--;
						delete threads[i];
						threads.removeAt(i);
						break;
					}
				}
			}

		}
		else{
			database->DeleteNonProcessedPhotoPaths(d);
		}
		d = database->GetFirstNonProcessedPhotoPaths();
	}

	for(int i = 0; i< threads.size(); i++){
		while(!threads[i]->isFinished());
		delete threads[i];
	}
	threads.clear();

	/*
	for(int k = 0 ; k < (*(threads[i]->getFaceList())).size() ;k++){
		for(int j = 0 ; j < faces->size() ; j++){
			r->compareFaces((*(threads[i]->getFaceList())).at(k),faces->at(j));
		}							
	}
	*/
}

string importPhotos::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}

void importPhotos::findImage(QString inp){
	QDir* direc = new QDir(inp);
	QFileInfoList fili = direc->entryInfoList();
	for(int i = 0 ; i < fili.size() ;i++){
		QFileInfo info = fili.at(i);
		if(info.fileName().compare(".")!=0 && info.fileName().compare("..")!=0){
			bool isDirec = info.isDir();
			if(isDirec){
				findImage(info.absoluteFilePath());
			}
			else{
				QString suf = info.suffix();
				if(suf.compare("jpg")==0)
					QDirectory->append(info.absoluteFilePath());
			}
		}
	}
}