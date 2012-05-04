#ifndef MDIFACE_H
#define MDIFACE_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QComboBox>
#include <QPushButton>
#include "db.h"
#include "face.h"
#include "rankingThread.h"
class mdiFace : public QWidget
{
	Q_OBJECT

public:
	mdiFace(QMdiArea*,face*,db* database,QList<face*> *faceList,rankingThread*);
	~mdiFace(void);

	 void setSubWinAdd(QMdiSubWindow* );
	 bool event(QEvent * e);
	 void setChildGeometry(int posX,int posY);
	 bool isClosed(){return closed;}
	 QMdiSubWindow* getWidgetaddr(){return widgetadd;}

protected:
     void closeEvent(QCloseEvent *event);
private slots:;
	void accaptButtonClicked();
	void rejectButtonClicked();
	void textChanged();
private:
	 QPixmap* transformImage();
	 string QStringToString(QString str);

	 QMdiArea* prnt;
	 QMdiSubWindow* widgetadd;
	 face* FaceObject;
	 QLabel* image;
//	 QEvent *e;
	 int posX, posY, w, h;
	 
	 bool closed;
	 QComboBox* labelComboBox;
	 QPushButton* labelPushButton;

	 QPushButton* accaptButton;
	 QPushButton* rejectButton;
	 db* database;
	 QList<face*> *faceList;
	 rankingThread* rt;
};

 #endif