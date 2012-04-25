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

#include "face.h"
class mdiFace : public QWidget
{
	Q_OBJECT

public:
	mdiFace(QMdiArea*,face*);
	~mdiFace(void);

	 void setSubWinAdd(QMdiSubWindow* );
	 bool event(QEvent * e);
	 void setChildGeometry(int posX,int posY);
protected:
     void closeEvent(QCloseEvent *event);
private slots:;
	void accaptButtonClicked();
	void rejectButtonClicked();
private:
	 QPixmap* transformImage();


	 QMdiArea* prnt;
	 QMdiSubWindow* widgetadd;
	 face* FaceObject;
	 QLabel* image;
	 QEvent *e;
	 int posX, posY, w, h;
	 QLineEdit* labelTextEdit;
	 QPushButton* accaptButton;
	 QPushButton* rejectButton;
};

 #endif