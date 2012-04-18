#include "mdiDSChild.h"


mdiDSChild::mdiDSChild(QMdiArea* parent,mdi* mdiArea)
{
	prnt = parent;
	this->mdiArea = mdiArea;
	setAttribute(Qt::WA_DeleteOnClose);

	image = new QLabel(this);
	image->setScaledContents(true);
	image->setMouseTracking(true);
}


mdiDSChild::~mdiDSChild(void)
{
}

void mdiDSChild::setPhoto(photo* photoObject){
	this->photoObject = photoObject;
	QPixmap* p = new QPixmap();
	p->convertFromImage(*photoObject->getDSImage());
	image->setPixmap(*p);
	w = p->width();
	h = p->height();
}

 void mdiDSChild::setChildGeometry(int posX,int posY){
	 posX += 50 - w/2;
	 posY += 50 - h/2;
	 widgetadd->setGeometry(QRect(posX,posY,w,h));
 }

 void mdiDSChild::setSubWinAdd(QMdiSubWindow* subwin){
	widgetadd = subwin;
 }

 bool mdiDSChild::event ( QEvent * e ){

	/*if(e->type() == QEvent::Move){
		 mdiArea->move(mdiArea->pos());
	 }
	 */
	if(e->type() == QEvent::MouseButtonDblClick){
//		mdi->closeAllSubWindows();
		mdiArea->addMdiPhoto(photoObject);
		/*
		child = new mdiPhoto(mdiArea,photoObject);

		child->setSubWinAdd(mdiArea->addSubWindow(child,Qt::FramelessWindowHint));
		child->setChildGeometry(0,0);
		child->show();
		*/
	}

	 return QWidget::event(e);
 }

 void mdiDSChild::closeEvent(QCloseEvent *event)
 {
	 delete prnt;
	 prnt = NULL;
	 delete widgetadd;
	 widgetadd = NULL;
	 delete photoObject;
	 photoObject = NULL;	

 }
