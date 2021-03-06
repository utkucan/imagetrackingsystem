#include "mdiDS.h"

mdiDS::mdiDS(QMdiArea* mdids,mdi* mdiArea,db* database)
{
	this->mdiArea = mdiArea;
	this->mdids = mdids;
	this->database = database;
	childPosx = 5;
	childPosy = 5;
}


mdiDS::~mdiDS(void)
{

}

void mdiDS::clearMdiDS(){
	while(photoWindows.size()>0){
		photoWindows[0]->close();
		delete photoWindows[0];
		photoWindows.removeFirst();
	}
	mdids->closeAllSubWindows();
	mdiArea->clearPhotos();
	childPosx = 5;
	childPosy = 5;
}

void mdiDS::addMdiDSChild(photo* photoObject){
	mdiDSChild *child = new mdiDSChild(mdids,mdiArea,database);
	child->setPhoto(photoObject);

	photoWindows.append(child);
	child->setSubWinAdd(mdids->addSubWindow(child,Qt::FramelessWindowHint));
	QScrollBar* scroll = mdids->horizontalScrollBar();
	
	int x = scroll->value();
	int cpx = childPosx - x;
	child->setChildGeometry(cpx,childPosy);
	childPosx += 105;
	child->show();
}

bool mdiDS::event(QEvent * e){
	// slider olay�n� bir bak
	//
	//
	return QWidget::event(e);
}