 #ifndef MDICHILD_H
 #define MDICHILD_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QFrame>
#include <QLineEdit>
#include "facedetect.h"
#include "face.h"
#include "popupDisplay.h"

 class MdiChild : public QWidget
 {
     Q_OBJECT

 public:
     MdiChild(QMdiArea*,face*);

	 void setSubWinAdd(QMdiSubWindow* );
	 bool event(QEvent * e);
	 void setChildGeometry(int posX,int posY);



     bool loadFile(const QString &fileName);
     bool save();
     bool saveAs();
     bool saveFile(const QString &fileName);
     QString userFriendlyCurrentFile();
	 QString currentFile() { return QString(FaceObject->getPath().c_str()); }
	 double ratio;
	 
 protected:
     void closeEvent(QCloseEvent *event);

 private slots:
     void documentWasModified();

 private:
	 QPixmap* transformImage();


     bool maybeSave();
     void setCurrentFile(const QString &fileName);
     QString strippedName(const QString &fullFileName);



	 QMdiArea* prnt;
	 QMdiSubWindow* widgetadd;
	 face* FaceObject;
	 QLabel* image;
	 QEvent *e;
	 int posX, posY, w, h;
	 QLineEdit* labelTextEdit;
 };

 #endif