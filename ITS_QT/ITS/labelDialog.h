#ifndef lABELDIALOG_H
#define lABELDIALOG_H

#include <QtGui\QDialog>
#include "ui_labelDialog.h"
#include <QtGui>
//#include <QDialog>
#include <QWidget>
#include <QKeyEvent>
#include "face.h"
#include <QComboBox>
#include <QPoint>
#include <string>
#include <QMdiSubWindow>
#include "db.h"
#include "ranking.h"
class labelDialog : public QDialog
{
	Q_OBJECT

public:
	labelDialog(QWidget *parent = 0, Qt::WFlags flags = 0,db* database = 0,QList<face*> *faceList =0);
	~labelDialog(void);
	void setFacePointer(face* fc);
	bool isclosed(){return closed;}
	void setPoint(QPoint);
	void setRelativePos(QPoint);
	QPoint getRelativePos(){return *rpos;}
protected:
    void closeEvent(QCloseEvent *event);
private slots:;
	void textChanged();
private:

	string QStringToString(QString);

	QMdiSubWindow* par;
	face* fc;
	bool closed;
	QPoint* rpos;
	Ui::LableDialog uiLd;
	db* database;
	QList<face*> *faceList;
};
#endif
