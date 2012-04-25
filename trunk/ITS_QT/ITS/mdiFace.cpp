#include "mdiFace.h"

 mdiFace::mdiFace(QMdiArea* parent,face* FaceObject)
 {
	 prnt = parent;
	 h = 0;
     setAttribute(Qt::WA_DeleteOnClose);
	 this->FaceObject = FaceObject;
	 
	 

	 image = new QLabel(this);
	 image->setScaledContents(true);
//	 image->setAlignment(Qt::AlignTop);
	 image->setPixmap(*transformImage());
	 image->setGeometry(0,0,w,h);

	 QIcon* accaptIcon = new QIcon("check.png");
	 accaptButton = new QPushButton(*accaptIcon,"",this);
	 accaptButton->setGeometry(0,h-20,20,20);
	 connect(accaptButton,SIGNAL(clicked()),this,SLOT(accaptButtonClicked()));

	 QIcon* rejectIcon = new QIcon("cross.png");
	 rejectButton = new QPushButton(*rejectIcon,"",this);
	 rejectButton->setGeometry(w-20,h-20,20,20);
	 connect(rejectButton,SIGNAL(clicked()),this,SLOT(rejectButtonClicked()));
	// h += 20;
	 /*
	 if(FaceObject->getLabel() == ""){

		labelTextEdit = new QLineEdit(this);
//		labelTextEdit->setAlignment(Qt::AlignBottom);
		labelTextEdit->setGeometry(0,h,w,20);
		labelTextEdit->setText("add label");
		//labelTextEdit->addAction("add label");
		h +=20;
	 }
	 */
//	 this->setContextMenuPolicy(Qt::CustomContextMenu);
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
	 delete prnt;
	 prnt = NULL;
	 delete widgetadd;
	 widgetadd = NULL;
	 delete FaceObject;
	 FaceObject = NULL;	
	 delete e;
	 e = NULL;
 }

void mdiFace::accaptButtonClicked(){
	int a = 5;
	//sonrasý database
}
void mdiFace::rejectButtonClicked(){
	int a = 5;
	//sonrasý database
}