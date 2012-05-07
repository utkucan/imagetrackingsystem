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
#include "rankingThread.h"
class mdi : public QMdiArea
{
public:
	mdi(QMdiArea*,db* database,rankingThread*);
	~mdi(void);
	void setFaceList(QList<face*> *faceList){this->faceList = faceList;}
	void addMdiFace(face*,int approved);
	void addMdiPhoto(photo*);
	void setPosition();
	void clearPhotos();
	int getFaceCount(){return faceWindows.size();}
	QList<int> displayedFaces();
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
	QList<face*> *faceList; 
	QMdiArea* mdiArea;
	db* database;
	rankingThread* rt;
	int approved;
};

 #endif