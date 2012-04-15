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
	mdiDSChild *child = new mdiDSChild(mdids,mdiArea,photoObject);
	
	photoWindows.append(child);
	child->setSubWinAdd(mdids->addSubWindow(child,Qt::FramelessWindowHint));
	child->setChildGeometry(childPosx,childPosy);
	childPosx += 105;
    child->show();
}

bool mdiDS::event(QEvent * e){
	// slider olayını bir bak
	//
	//
	return QWidget::event(e);
}