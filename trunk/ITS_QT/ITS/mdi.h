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
#include "db.h"
class mdi : public QMdiArea
{
public:
	mdi(QMdiArea*,db* database);
	~mdi(void);
	void addMdiFace(face*);
	void addMdiPhoto(photo*);
	void setPosition();
	void clearPhotos();
	
//	bool event(QEvent * e);
	void updateFace();
	void reDisplayLastPhoto();
	void faceDisplayGeomety();
	void photoDisplayGeomety();
private:
	QImage* IplImage2QImage(const IplImage *iplImage);

	int childPosx,childPosy;
	QList<mdiFace*> faceWindows;
	QList<mdiPhoto*> photoWindows;
	QList<photo*> photolst;
	QMdiArea* mdiArea;
	db* database;
};

 #endif