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
#include <QTreeWidget>
#include <QTreeWidgetItem>
 #include <QMoveEvent>

#include <stdio.h>
#include <fstream>

#include "mdiDS.h"
#include "mdi.h"
#include "importPhotos.h"
#include "photo.h"
#include "face.h"

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
private:
	void displayFace();
	void displayPhoto(int);
	void initializeTree();
	void findImage(QString inp);
	bool event(QEvent * e);


	int faceCount;
	QList<photo*> *photoList;
	mdi* mdiArea;
	mdiDS* mdids;
	QTreeWidget* treeWidget;
	QTreeWidgetItem* photoItem;
	QTreeWidgetItem* labelItem;
	QList<QString>* allImagesList;
//	analyzer* imageAnalyzer;
	Ui::ITSClass ui;
};

#endif // ITS_H
