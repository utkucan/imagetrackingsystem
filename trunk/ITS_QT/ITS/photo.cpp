#include "photo.h"


photo::photo(QString path,QList<face*>*faces)
{
	this->path = QStringToString(path);
	this->faceList = faces;
	downSampleImage();
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

void photo::downSampleImage(){
	QImage* tmp = new QImage(path.c_str());
	if(tmp->height()>tmp->width()){
		downSample = new QImage(tmp->scaledToHeight( 100,Qt::FastTransformation));
	}else{
		downSample = new QImage(tmp->scaledToWidth( 100,Qt::FastTransformation));
	}
	delete tmp;
}