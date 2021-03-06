#include "mdiFace.h"

 mdiFace::mdiFace(QMdiArea* parent,face* FaceObject,db* database,QList<face*> *faceList,rankingThread* rt,int approved)
 {
	parent->setFont(QFont("Fantasy", 10, QFont::Bold));
	 prnt = parent;
	 h = 0;
     setAttribute(Qt::WA_DeleteOnClose);
	 this->FaceObject = FaceObject;
	 this->database = database;
	 this->faceList =faceList;
	 this->rt =rt;
	 closed = false;

	 image = new QLabel(this);
	 image->setScaledContents(true);
//	 image->setAlignment(Qt::AlignTop);
	 image->setPixmap(*transformImage());
	 image->setGeometry(0,0,w,h);

	 if(approved == 0){//if(FaceObject->getLabel() == "Unknown"){
		 labelComboBox = new QComboBox(this);
		 labelComboBox->setGeometry(0,h,w-20,20);
		 //labelComboBox->setGeometry(0,h,w-40,20);
		 labelComboBox->setEditable(true);
		 
		 labelComboBox->addItems(database->getSuggested(FaceObject->getID()));
		 
		 labelComboBox->setCurrentIndex(-1);
		// labelComboBox->setMaxCount(3);
		 labelComboBox->setMaxVisibleItems(3);
		 //labelComboBox->setMaximumHeight(40);
	//	 connect(labelComboBox,SIGNAL(keyPressEvent(QKeyEvent*)),this,SLOT(textChanged()));
		
		 QIcon* accaptIcon = new QIcon("check.png");
		 accaptButton = new QPushButton(*accaptIcon,"",this);
		 accaptButton->setGeometry(w-20,h,20,20);
		 connect(accaptButton,SIGNAL(clicked()),this,SLOT(textChanged()));
		
		 QIcon* rejectIcon = new QIcon("cross.png");
		 rejectButton = new QPushButton(*rejectIcon,"",this);
		 rejectButton->setGeometry(w-20,0,20,20);
		// rejectButton->setGeometry(w-20,h,20,20);
		 connect(rejectButton,SIGNAL(clicked()),this,SLOT(accaptButtonClicked()));

		 /*
		 labelPushButton = new QPushButton(this);
		 labelPushButton->setGeometry(w-20,h,20,20);
		 labelPushButton->setText("ok");
		 */
		// connect(labelPushButton,SIGNAL(clicked()),this,SLOT(textChanged()));
		 /*
		labelTextEdit = new QLineEdit(this);
		labelTextEdit->setGeometry(0,h,w,20);
		labelTextEdit->setText("add label");
	//	connect(accaptButton,SIGNAL(clicked()),this,SLOT(accaptButtonClicked()));
		*/
		h +=20;
	 }else{

		 QIcon* accaptIcon = new QIcon("check.png");
		 accaptButton = new QPushButton(*accaptIcon,"",this);
		 accaptButton->setGeometry(0,h-20,20,20);
		 connect(accaptButton,SIGNAL(clicked()),this,SLOT(accaptButtonClicked()));

		 QIcon* rejectIcon = new QIcon("cross.png");
		 rejectButton = new QPushButton(*rejectIcon,"",this);
		 rejectButton->setGeometry(w-20,h-20,20,20);
		 connect(rejectButton,SIGNAL(clicked()),this,SLOT(rejectButtonClicked()));
	 }
 }
 mdiFace::~mdiFace(void){
 
 }

 void mdiFace::setChildGeometry(int posX,int posY){
	 this->posX = posX;
	 this->posY = posY;
	 widgetadd->setGeometry(QRect(posX,posY,w,h));
 }

 void mdiFace::setSubWinAdd(QMdiSubWindow* subwin){
	widgetadd = subwin;
 }

 bool mdiFace::event ( QEvent * e ){
	 if(e->type() == QEvent::KeyPress){
		 int key = ((QKeyEvent*)e)->key();
		 if(key == 16777220 || key == 16777221){
			textChanged();
		 }
	 }
	 /*
	 if(e->type() == QEvent::MouseButtonPress){
		 if( ((QMouseEvent*)e)->button() == Qt::RightButton){
			QPoint globalPos = this->mapToGlobal(((QMouseEvent*)e)->pos());
			QMenu myMenu(this);
			if(FaceObject->getLabel() == ""){
				myMenu.addAction("add label");
				QAction* selectedItem = myMenu.exec(globalPos);
				if(selectedItem->text() == "add label"){
					int a = 5;
				}
			}else{
				
			}
		 }
	 }

	 if(e->type() == QEvent::MouseButtonDblClick){
		 QImage *img = new QImage ( FaceObject->getPath().c_str() );
		 
		 popupDisplay* popup = new popupDisplay(img);
		 popup->show();
	 }
	 */
	 return QWidget::event(e);
 }

 QPixmap* mdiFace::transformImage(){
	QImage* img = FaceObject->getImage();

	QPixmap* p = new QPixmap();
	p->convertFromImage(*img,Qt::AutoColor);
	this->w = FaceObject->getTransformedWidth();
	this->h += FaceObject->getTransformedHeight();
	return p;
 }

void mdiFace::closeEvent(QCloseEvent *event)
 {
	 closed = true;
	 widgetadd->close();
	 
	 prnt = NULL;
	 widgetadd = NULL;
	 FaceObject = NULL;
 }

void mdiFace::accaptButtonClicked(){

	database->updateHasFaces(FaceObject->getID(),QString(FaceObject->getLabel().c_str()),FaceObject->getPhotoId(),1);
	database->deleteFromSuggested(FaceObject->getID());
	rt->start();
	/*
	ranking* r = new ranking(database,faceList);
	r->setFaceId(0);
	r->start();
	*/
	this->close();
}
void mdiFace::rejectButtonClicked(){
	// reject edince nolcak??????
	
	database->insertNonEqual(FaceObject->getID(),QString(FaceObject->getLabel().c_str()));
	FaceObject->setLabel("Unknown");
	database->updateHasFaces(FaceObject->getID(),"Unknown",FaceObject->getPhotoId(),0);
	database->deleteFromSuggested(FaceObject->getID());
	rt->start();
	/*
	ranking* r = new ranking(database,faceList);
	r->setFaceId(FaceObject->getID());
	r->start();
	*/
	this->close();
}

 void mdiFace::textChanged(){
	QString lbl = labelComboBox->currentText();
	if(lbl == "")
		return;
	lbl = lbl.lower();
	string l = QStringToString(lbl);
	FaceObject->setLabel(l);
	database->updateHasFaces(FaceObject->getID(),lbl,FaceObject->getPhotoId(),1);
	database->deleteFromSuggested(FaceObject->getID());
	rt->start();
	/*
	ranking* r = new ranking(database,faceList);
	r->setFaceId(0);
	r->start();
	*/
	this->close();
}

string mdiFace::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}
