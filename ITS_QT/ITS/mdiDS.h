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

class mdiDS : public QMdiArea
{
public:
	mdiDS(QMdiArea*,mdi*);
	~mdiDS(void);
	void addMdiDSChild(photo* photoObject);
	bool event(QEvent * e);
private:
	int childPosx,childPosy;
	QList<mdiDSChild*> photoWindows;
	QMdiArea* mdids;
	mdi* mdiArea;
};

 #endif