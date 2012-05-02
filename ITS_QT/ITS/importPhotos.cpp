#include "importPhotos.h"

importPhotos::importPhotos(QString dir,db* database,QList<photo*> *photos,QList<face*> *faces){
	this->database = database;
	this->dir = dir;
	this->photos = photos;
	this->faces = faces;
	QDirectory = new QStringList();
	m = new QMutex();
	maxNumOfThread = 1;
	flag = true;
	
}

importPhotos::importPhotos(QStringList QDirectory,db* database,QList<photo*> *photos,QList<face*> *faces)
{
	this->QDirectory = new QStringList(QDirectory);
	this->photos = photos;
	this->database = database;
	this->faces = faces;
	m = new QMutex();
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
	if(flag)
		findImage(dir);
	ranking *r = new ranking(database);
	QList<analyzer*> threads;
	int numOfThread = 0;
//	QList<photo*> *photos;
	for(int j = 0 ; j< QDirectory->count(); j++){
		if(!database->photoExist((*QDirectory)[j])){
			string filename = QStringToString((*QDirectory)[j]);
			QList<face*> *tmpLst = new QList<face*>();

	//		photo* p = new photo((*QDirectory)[j],tmpLst);
	//		photos->append(p);

			analyzer* imageAnalyzer = new analyzer(tmpLst,filename);
			imageAnalyzer->start();
			threads.append(imageAnalyzer);
			numOfThread++;

			while(numOfThread == maxNumOfThread){
				for(int i = 0; i< threads.size(); i++){
					if(threads[i]->isFinished()){
					//	QList<face*> *tmpLst = threads[i]->getFaceList();
						if(threads[i]->getFaceList()->size()>0){
							photo* p = new photo( QString( threads[i]->getFileName().c_str()),threads[i]->getFaceList());
							faces->append(*(threads[i]->getFaceList()));
							photos->append(p);
							database->insertIntoPhoto(p);
							
							for(int j = 0 ; j<(*(threads[i]->getFaceList())).size(); j++){
								for(int k = 0 ;k<faces->size();k++)
									r->compareFaces((*(threads[i]->getFaceList())).at(j),faces->at(k));
							}

						}
					//	int pos = photos->size() - threads.size() + i;
					//	database->insertIntoPhoto((*photos)[pos]);
						numOfThread--;
						delete threads[i];
						threads.removeAt(i);
						break;
					}
				}
			}

		}
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