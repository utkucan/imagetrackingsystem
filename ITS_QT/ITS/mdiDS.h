 #ifndef MDIDS_H
 #define MDIDS_H
#include <QLabel>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QDebug>
#include <QScrollBar>

#include "mdi.h"
#include "mdiDSChild.h"
#include "db.h"
//#include "treeWid.h"

class mdiDS : public QMdiArea
{
public:
	mdiDS(QMdiArea*,mdi*,db* database);
	~mdiDS(void);
	void addMdiDSChild(photo* photoObject);
	bool event(QEvent * e);
	void clearMdiDS();
	void setvisible(bool flag){mdids->setVisible(flag);}
private:
	int childPosx,childPosy;
	QList<mdiDSChild*> photoWindows;
	QMdiArea* mdids;
	mdi* mdiArea;
	db* database;
};

 #endif