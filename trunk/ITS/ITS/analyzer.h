#ifndef ANALYZER_H
#define ANALYZER_H

#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QDebug>
#include "facedetect.h"
#include "mdi.h"
#include "face.h"

class analyzer
{
public:
	analyzer(QList<face*>*);
	~analyzer(void);
	void analyz(string imagePath,int idStart);

private:
	QImage* IplImage2QImage(const IplImage *iplImage);
	QList<face*> *faceList;
};
#endif
