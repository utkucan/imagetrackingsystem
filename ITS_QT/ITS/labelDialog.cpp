#include "labelDialog.h"


labelDialog::labelDialog(QWidget *parent, Qt::WFlags flags,db* database,QList<face*> *faceList,rankingThread* rt): QDialog(parent, flags)
{
//	setParent(parent);
//	setWindowFlags(Qt::FramelessWindowHint);
//	setEditable(true);
//	setGeometry(position.x(),position.y(),100,30);
//	setAttribute(Qt::WA_DeleteOnClose);
	par = (QMdiSubWindow*)parent;
	uiLd.setupUi(this);
	this->database = database;
	this->faceList = faceList;
	this->rt = rt;
	//Qt::FramelessWindowHint
	closed = false;
	rpos = new QPoint();
}

labelDialog::~labelDialog(void)
{
}

void labelDialog::setPoint(QPoint position){
	setGeometry(position.x(),position.y(),150,25);
}

void labelDialog::textChanged(){
	QString lbl = uiLd.comboBox->currentText();
	if(lbl == "" || lbl == QString(fc->getLabel().c_str()))
		return;
	lbl = lbl.lower();
	string l = QStringToString(lbl);
	this->fc->setLabel(l);
	database->updateHasFaces(fc->getID(),lbl,fc->getPhotoId(),1);
	database->DeletePersonHasNoFace();
	database->deleteFromSuggested(fc->getID());
	this->close();
	rt->start();
	/*
	ranking* r = new ranking(database,faceList);
	r->setFaceId(0);
	r->start();
	*/
	par->close();
}

void labelDialog::setFacePointer(face* fc){
	this->fc = fc;
	uiLd.comboBox->addItems((QStringList)(database->getSuggested(fc->getID())));
	if(fc->getLabel() != "Unknown")
		uiLd.comboBox->setCurrentText(QString(fc->getLabel().c_str()));
	else
		uiLd.comboBox->setCurrentIndex(-1);
	uiLd.comboBox->setMaxCount(3);
}
string labelDialog::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}

 void labelDialog::closeEvent(QCloseEvent *event)
 {
	 fc = NULL;
	 delete rpos;
	 closed = true;
 }

 void labelDialog::setRelativePos(QPoint p){
	 rpos->setX(p.x());
	 rpos->setY(p.y());
 }