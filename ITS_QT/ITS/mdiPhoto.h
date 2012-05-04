#ifndef MDIPHOTO_H
#define MDIPHOTO_H

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
#include "face.h"
#include "photo.h"
#include "labelDialog.h"
#include "db.h"
#include "ranking.h"
class mdiPhoto : public QWidget
{
	Q_OBJECT

public:
	mdiPhoto(QMdiArea*,photo*,db* database,QList<face*> *faceList);
	~mdiPhoto(void);
	void setSubWinAdd(QMdiSubWindow* );
	bool event(QEvent * e);
	void setChildGeometry(int posX,int posY);
	void setPosition();
	bool isClosed(){return closed;}
protected:
    void closeEvent(QCloseEvent *event);
private:

	void onFaceRect(int,QPoint);

	bool closed;
	QMdiArea* prnt;
	QMdiSubWindow* widgetadd;
	photo* photoObject;
	QLabel* image;
	QEvent *e;
	int posX, posY, w, h;
	QList<QRect*> *faceRects;
	QList<face*> *faceList2;
//	QList<QString*> *labelList;
	QPixmap* p;
	labelDialog* ld;
	db* database;
	QList<face*> *faceList;
};

 #endif