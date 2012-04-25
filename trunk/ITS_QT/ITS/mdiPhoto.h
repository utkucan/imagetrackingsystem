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
class mdiPhoto : public QWidget
{
	Q_OBJECT

public:
	mdiPhoto(QMdiArea*,photo*,db* database);
	~mdiPhoto(void);
	void setSubWinAdd(QMdiSubWindow* );
	bool event(QEvent * e);
	void setChildGeometry(int posX,int posY);
	void setPosition();
protected:
    void closeEvent(QCloseEvent *event);
private:

	void onFaceRect(int,QPoint);

	QMdiArea* prnt;
	QMdiSubWindow* widgetadd;
	photo* photoObject;
	QLabel* image;
	QEvent *e;
	int posX, posY, w, h;
	QList<QRect*> *faceRects;
	QList<face*> *faceList;
//	QList<QString*> *labelList;
	QPixmap* p;
	labelDialog* ld;
	db* database;
};

 #endif