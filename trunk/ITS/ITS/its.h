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
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "mdi.h"
#include "analyzer.h"
using namespace std;

class ITS : public QMainWindow
{
	Q_OBJECT

public:
	ITS(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ITS();
private slots:;
	void on_actionKlasor_triggered();
private:
	string QStringToString(QString);
	void displayFace();
	
	int faceCount;
	QList<face*> *faceList;
	mdi* mdiArea;
	analyzer* imageAnalyzer;
	Ui::ITSClass ui;
};

#endif // ITS_H
