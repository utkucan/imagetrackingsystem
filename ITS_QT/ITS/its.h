#ifndef ITS_H
#define ITS_H

#include <QtGui/QMainWindow>
#include "ui_its.h"
#include <QImage>
#include <string>
#include <iostream>
#include <QDebug>
#include <QString>
#include <QtGui>
#include <QFile>
#include <QImage>
#include <QMdiArea>
#include <QCheckBox>
#include <QListWidget>
#include <QList>

#include <QMoveEvent>
#include <QTimer>

#include <stdio.h>
#include <fstream>

#include "mdiDS.h"
#include "mdi.h"
#include "importPhotos.h"
#include "photo.h"
#include "face.h"
#include "db.h"
#include "treeWid.h"
#include "matlab.h"
#include "popupDisplay.h"
#include "rankingThread.h"
using namespace std;

class ITS : public QMainWindow
{
	Q_OBJECT

public:
	ITS(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ITS();
private slots:;
	void on_actionKlasor_triggered();
	void on_actionSearch_HardDisk_triggered();
	void controlList();
	void treeWidgetSelectionChange();
	void updateFace();
	void showAbout();
	void checkRecognizedFaces();
	void closeEvent(QCloseEvent *event);
private:
	
//	void displayFace(int);
//	void displayPhoto(int);
//	void initializeTree();
//	void findImage(QString inp,QStringList*);
	bool event(QEvent * e);

	
//	int faceCount;
//	int photoPos;
	//importPhotos* ip;
	db *database;
//	QList<photo*> *photoList;
	mdi* mdiArea;
	mdiDS* mdids;
	treeWid* treeWidget;
	matlab* m;
	rankingThread* rt;
	QTimer* recognizedTimer;
	/*
	QTreeWidget* treeWidget;
	QTreeWidgetItem* unlabeledPhotoItem;
	QTreeWidgetItem* labelItem;
	*/
//	QList<QString>* allImagesList;
//	analyzer* imageAnalyzer;
	Ui::ITSClass ui;
	QList<importPhotos*> ipList;
};

#endif // ITS_H
