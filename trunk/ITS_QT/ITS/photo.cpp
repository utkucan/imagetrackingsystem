#include "photo.h"


photo::photo(QString path,QList<face*>*faces)
{
	this->path = QStringToString(path);
	this->faceList = faces;
}


photo::~photo(void)
{
	faceList->clear();
	delete faceList;
	faceList = NULL;
}

QList<face*>* photo::getFaces(){return faceList;}

string photo::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}