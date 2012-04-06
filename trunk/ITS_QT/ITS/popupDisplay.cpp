#include "popupDisplay.h"


popupDisplay::popupDisplay(/*QFrame* frm,*/QImage* image)
{
	setWindowState(Qt::WindowState::WindowFullScreen);
	setAttribute(Qt::WA_DeleteOnClose);

	QPixmap pixmap(size());
	pixmap.fill(Qt::transparent);
	QPainter::setRedirected(this, &pixmap);
	QPaintEvent pe(rect());
	paintEvent(&pe);
	QPainter::restoreRedirected(this);
	setMask(pixmap.mask());
	

	lbl = new QLabel(this);
	
//	setParent(frm);
	

//	setWindowState(Qt::WindowState::WindowFullScreen);
	QPixmap* p = new QPixmap();
	p->convertFromImage(*image,Qt::AutoColor);
	this->pix = p;

//	frame->resize(pix->width(),pix->height());
	lbl->setPixmap(*pix);
//	lbl->adjustSize();
	ratio = 1;
//	adjustSize();
//	frame->adjustSize();
//	show();
}


popupDisplay::~popupDisplay(void)
{

}

 void popupDisplay::closeEvent(QCloseEvent *event)
 {
	delete pix;
	pix=NULL;
	delete lbl;
	lbl = NULL;
 }

 bool popupDisplay::event ( QEvent * e ){
	
	 if(e->type() == QEvent::KeyPress){
		
		 int key = ((QKeyEvent*)e)->key();
		 if( ((QKeyEvent*)e)->key() == 16777235){
			 ratio += ratio/10;
			 int h = pix->height();
			 QPixmap* p = new QPixmap(pix->scaledToHeight ( (int)(h * ratio),Qt::FastTransformation  ));
			 lbl->setPixmap(*p);
			 lbl->adjustSize();
		 }else if(((QKeyEvent*)e)->key() == 16777237){
			 if(ratio>0.01){
				 ratio -= ratio/10;
				 int h = pix->height();
				 QPixmap* p = new QPixmap(pix->scaledToHeight ( (int)(h * ratio),Qt::FastTransformation  ));
				 lbl->setPixmap(*p);
				 lbl->adjustSize();
			 }
		 }else if(((QKeyEvent*)e)->key() == 16777216){
			 close();
		 }
	 }


	/*
	if(e->type() == QEvent::Wheel){
		
		int numDegrees = ((QWheelEvent*)e)->delta()/8;
		double numSteps = (double)numDegrees/150;
		int w = pix->width();
		int h = pix->height();
		ratio += numSteps;
		if(ratio<0){
			ratio = 0.1;
		}
		
		QPixmap* p = new QPixmap(pix->scaledToHeight ( (int)(h * ratio),Qt::FastTransformation  ));
//		frame->resize(p->width(),p->height());
		lbl->setPixmap(*p);
//		adjustSize();
	}
	*/

	return QWidget::event(e);
 
 }
