 #ifndef MDI_H
 #define MDI_H
#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QDebug>

#include "mdiChild.h"
#include "mdiFace.h"
#include "mdiPhoto.h"
#include "face.h"
class mdi : public QMdiArea
{
public:
	mdi(QMdiArea*);
	~mdi(void);
	void addMdiFace(face*);
	void addMdiPhoto(photo*);
	void setPosition();
private:
	QImage* IplImage2QImage(const IplImage *iplImage);

	int childPosx,childPosy;
	QList<mdiFace*> faceWindows;
	QList<mdiPhoto*> photoWindows;
	QMdiArea* mdiArea;
};

 #endif