 #ifndef MDI_H
 #define MDI_H
#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QDebug>
#include "mdiChild.h"
#include "face.h"
class mdi : public QMdiArea
{
public:
	mdi(QMdiArea*);
	~mdi(void);
	void addMdiChild(face*);
private:
	QImage* IplImage2QImage(const IplImage *iplImage);


	int childPosx,childPosy;
	QList<MdiChild*> childWindows;
	QMdiArea* mdiArea;
};

 #endif