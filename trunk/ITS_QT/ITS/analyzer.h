#ifndef ANALYZER_H
#define ANALYZER_H

#include <QThread>
#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QDebug>

#include "facedetect.h"
#include "mdi.h"
#include "face.h"

class analyzer : public QThread
{
public:
	analyzer(QList<face*>*,string);
	~analyzer(void);
	void run();
	QList<face*>* getFaceList(){return faceList;}
	string getFileName(){return imagePath;}
//	void analyz(string imagePath);
private:

	QImage* transformImage(QImage*);
	QImage* IplImage2QImage(const IplImage *iplImage);
	void saveface(QImage*);

	QList<face*> *faceList;
	string imagePath;
	int count;
};
#endif
