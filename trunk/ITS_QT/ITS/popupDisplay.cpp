#include "popupDisplay.h"


popupDisplay::popupDisplay(QImage* image)
{
	QPixmap* p = new QPixmap();
	p->convertFromImage(*image,Qt::AutoColor);
	frame= new QSplashScreen(*p);
	
}


popupDisplay::~popupDisplay(void)
{
	frame->close();
	delete frame;
}
void popupDisplay::run(){

	frame->show();
	while(true);

}

