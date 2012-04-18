#include "mdiDS.h"

mdiDS::mdiDS(QMdiArea* mdids,mdi* mdiArea)
{
	this->mdiArea = mdiArea;
	this->mdids = mdids;
	childPosx = 5;
	childPosy = 5;
}


mdiDS::~mdiDS(void)
{
}

void mdiDS::addMdiDSChild(photo* photoObject){
	
	mdiDSChild *child = new mdiDSChild(mdids,mdiArea);
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
	// slider olayını bir bak
	//
	//
	return QWidget::event(e);
}