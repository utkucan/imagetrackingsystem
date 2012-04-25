#include "importPhotos.h"

importPhotos::importPhotos(QStringList QDirectory,QList<photo*> *photos,db* database)
{
	this->QDirectory = new QStringList(QDirectory);
	this->photos = photos;
	this->database = database;
	m = new QMutex();
	maxNumOfThread = 1;//QDirectory->size();
}


importPhotos::~importPhotos(void)
{
	
	QDirectory->clear();
	delete QDirectory;
	QDirectory = NULL;
	photos = NULL;
	
}

void importPhotos::run(){
	QList<analyzer*> threads;
	int numOfThread = 0;
	for(int j = 0 ; j< QDirectory->count(); j++){
		string filename = QStringToString((*QDirectory)[j]);
		QList<face*> *tmpLst = new QList<face*>();

		photo* p = new photo((*QDirectory)[j],tmpLst);
		photos->append(p);

		analyzer* imageAnalyzer = new analyzer(tmpLst,filename);
		imageAnalyzer->start();
		threads.append(imageAnalyzer);
		numOfThread++;

		while(numOfThread == maxNumOfThread){
			for(int i = 0; i< threads.size(); i++){
				if(threads[i]->isFinished()){
					int pos = photos->size() - threads.size() + i;
					database->insertIntoPhoto((*photos)[pos]);
					numOfThread--;
					delete threads[i];
					threads.removeAt(i);
					break;
				}
			}
		}

	}

	for(int i = 0; i< threads.size(); i++){
		while(!threads[i]->isFinished());
		delete threads[i];
	}
	threads.clear();
}

string importPhotos::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}