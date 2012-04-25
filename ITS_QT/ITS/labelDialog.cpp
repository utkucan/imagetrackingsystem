#include "labelDialog.h"


labelDialog::labelDialog(QWidget *parent, Qt::WFlags flags,db* database): QDialog(parent, flags)
{
//	setParent(parent);
//	setWindowFlags(Qt::FramelessWindowHint);
//	setEditable(true);
//	setGeometry(position.x(),position.y(),100,30);
//	setAttribute(Qt::WA_DeleteOnClose);
	uiLd.setupUi(this);
	this->database = database;
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
	string l = QStringToString(lbl);
	this->fc->setLabel(l);
	database->updateHasFaces(fc->getID(),lbl,fc->getPhotoId());
	this->close();
}

void labelDialog::setFacePointer(face* fc){
	this->fc = fc;
	QString lbl(fc->getLabel().c_str());
	uiLd.comboBox->addItem(lbl);
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