#ifndef MDIDSCHILD_H
#define MDIDSCHILD_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QFrame>
#include <QLineEdit>
#include <QShowEvent>
#include <QPainter>
#include <QToolTip>
#include <QMenu>
#include "photo.h"
#include "mdi.h"
class mdiDSChild : public QWidget
{
public:
	mdiDSChild(QMdiArea*,mdi*,photo*);
	~mdiDSChild(void);

	void setSubWinAdd(QMdiSubWindow* );
	bool event(QEvent * e);
	void setChildGeometry(int posX,int posY);

protected:
    void closeEvent(QCloseEvent *event);

private:
	QMdiArea* prnt;
	mdi* mdiArea;
	QMdiSubWindow* widgetadd;
	photo* photoObject;
	QLabel* image;
	mdiPhoto *child;
	int w,h;
};

 #endif