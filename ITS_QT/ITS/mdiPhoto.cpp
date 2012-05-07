#include "mdiPhoto.h"


mdiPhoto::mdiPhoto(QMdiArea* parent,photo* photoObject,db* database,QList<face*> *faceList2,rankingThread* rt)
{
	prnt = parent;
	setAttribute(Qt::WA_DeleteOnClose);
	setMouseTracking(true);
	this->photoObject = photoObject;
	this->database = database;
	this->faceList2 = faceList2;
	this->rt =rt;
	image = new QLabel(this);
	image->setScaledContents(true);
	image->setMouseTracking(true);
	ld = NULL;
	p = new QPixmap(photoObject->getPath().c_str());
	closed = false;
	double ratio = 1;
	QPixmap* tmp;


	if(p->height() > 400){
		ratio = (double)400 / (double)p->height();
		tmp = new QPixmap(p->scaledToHeight( 400,Qt::FastTransformation));
		delete p;
		p = new QPixmap(*tmp);
		delete tmp;
	}

	/*
	if(p->height()>p->width()){
		if(p->height() > 400){
			ratio = (double)400 / (double)p->height();
			tmp = new QPixmap(p->scaledToHeight( 400,Qt::FastTransformation));
			delete p;
			p = new QPixmap(*tmp);
			delete tmp;
		}
	}else{
		if(p->width() > 700){
			ratio = (double)700 / (double)p->width();
			tmp = new QPixmap(p->scaledToWidth( 700,Qt::FastTransformation));
			delete p;
			p = new QPixmap(*tmp);
			delete tmp;
		}
	}
	*/
	
	faceRects = new QList<QRect*>();
	faceList = new QList<face*>();
//	labelList = new QList<QString*>();
	faceList = photoObject->getFaces();
	for(int i = 0; i<faceList->size(); i++){
		int x = (*faceList)[i]->getX() * ratio;
		int y = (*faceList)[i]->getY() * ratio;
		int wf = (*faceList)[i]->getWidth()* ratio;
		int hf = (*faceList)[i]->getHeight()* ratio;
		QRect* r = new QRect(x,y,wf,hf);
		faceRects->append(r);
//		labelList->append( new QString( (*fl)[i]->getLabel().c_str()));
	}
	/*
	for(int i = 0 ; i< faceRects->size(); i++){
			QPainter pnter;
			pnter.begin((QPaintDevice*)p);
			pnter.setPen(QPen(QColor(Qt::red)));
			pnter.setBrush(QBrush(QColor(Qt::red), Qt::NoBrush));
			pnter.drawRect(*(*faceRects)[i]);
			pnter.end();
	}*/

	image->setPixmap(*p);
	w = p->width();
	h = p->height();
	setAttribute(Qt::WA_AlwaysShowToolTips);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
}


mdiPhoto::~mdiPhoto(void)
{
}

 void mdiPhoto::setChildGeometry(int posX,int posY){
	posX = 350 - w/2;
	posY = 0;
	 widgetadd->setGeometry(QRect(posX,posY,w,h));
 }

 void mdiPhoto::setSubWinAdd(QMdiSubWindow* subwin){
	widgetadd = subwin;
 }

 void mdiPhoto::onFaceRect(int i,QPoint globalPos){
	QPainter pnter;
	QPixmap* pixptr = new QPixmap(*p);
	pnter.begin((QPaintDevice*)pixptr);
	pnter.setPen(QPen(QColor(Qt::red)));
	pnter.setBrush(QBrush(QColor(Qt::red), Qt::NoBrush));
	pnter.drawRect(*(*faceRects)[i]);
	pnter.end();
	image->setPixmap(*pixptr);
	delete pixptr;
			
	if( (*faceList)[i]->getLabel() == "Unknown" ){
		QToolTip::showText(globalPos,"Unknown",image);
	}else{

		QToolTip::showText(globalPos,QString((*faceList)[i]->getLabel().c_str()));
	}
 }

 bool mdiPhoto::event ( QEvent * e ){

	if(e->type() == QEvent::MouseButtonPress){
		if(ld != NULL){
			if(!ld->isclosed()){
				ld->close();
				delete ld;
			}
			ld = NULL;
		}
		if( ((QMouseEvent*)e)->button() == Qt::RightButton){
			for(int i = 0 ; i< faceRects->size(); i++){
				int x = ((QMouseEvent*)e)->x();
				int y = ((QMouseEvent*)e)->y();
				if( (*faceRects)[i]->contains(x,y)){
					
					QPoint globalPos = this->mapToGlobal(((QMouseEvent*)e)->pos());
					QMenu myMenu(this);
					if((*faceList)[i]->getLabel() == "Unknown"){
						myMenu.addAction("add label");
						QAction* selectedItem = myMenu.exec(globalPos);
						if(selectedItem != NULL && selectedItem->text() == "add label"){
							ld = new labelDialog(widgetadd,Qt::FramelessWindowHint,database,faceList2,rt);
							//widgetadd->disac
							ld->setFacePointer((*faceList)[i]);
							ld->setPoint(globalPos);
							ld->setRelativePos(((QMouseEvent*)e)->pos());
							ld->show();
						}
					}else{
						myMenu.addAction("change label");
						QAction* selectedItem = myMenu.exec(globalPos);
						if(selectedItem != NULL && selectedItem->text() == "change label"){
							ld = new labelDialog(widgetadd,Qt::FramelessWindowHint,database,faceList2,rt);
							ld->setFacePointer((*faceList)[i]);
							ld->setPoint(globalPos);
							ld->setRelativePos(((QMouseEvent*)e)->pos());
							ld->show();
						}
					}
					break;
				}
			}
		}
	 }else if( e->type() == QEvent::MouseMove){
		bool set = false;
		for(int i = 0 ; i< faceRects->size(); i++){
			int x = ((QMouseEvent*)e)->x();
			int y = ((QMouseEvent*)e)->y();
			if( (*faceRects)[i]->contains(x,y)){
				int labelX = (*faceRects)[i]->x() + ( (*faceRects)[i]->width() / 2 );
				int labelY = (*faceRects)[i]->y() + (*faceRects)[i]->height();
				QPoint labelPos(labelX,labelY);
				QPoint globalPos = this->mapToGlobal(labelPos);
//				QPoint globalPos = this->mapToGlobal(((QMouseEvent*)e)->pos());
				onFaceRect(i,globalPos);
				set = true;
				break;
			}
		}
		if(!set){
			image->setPixmap(*p);
			QToolTip::hideText();
		}
	}
/*	
	if(e->type() == QEvent::Paint){

		if(ld != NULL){
			QPoint lp = ld->getRelativePos();
			QPoint gp = widgetadd->mapToGlobal(lp);
			QPoint ldp = ld->pos();
			if(ld->pos() != gp){
				ld->setPoint(gp);
				ld->repaint();
			}
		}
	 }
*/	 
	 return QWidget::event(e);
 }

 void mdiPhoto::closeEvent(QCloseEvent *event)
 {
	 prnt = NULL;
	 widgetadd = NULL;
	 photoObject = NULL;	
	 e = NULL;
	 
	 image->clear();
	 delete image;
	 image = NULL;

	 faceRects->clear();
	 delete faceRects;
	 faceRects = NULL;

	 faceList = NULL;

	 delete p;
	 p = NULL;
	 if(ld != NULL){
		 if(!ld->isclosed()){
			 ld->close();
			 delete ld;
		 }
		 ld = NULL;
	 }
	 closed = true;
 }

 void mdiPhoto::setPosition(){
	if(ld != NULL){
		QPoint lp = ld->getRelativePos();
		QPoint gp = widgetadd->mapToGlobal(lp);
		QPoint ldp = ld->pos();
		if(ld->pos() != gp){
			ld->setPoint(gp);
			ld->repaint();
		}
	}
 }