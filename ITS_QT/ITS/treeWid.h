#ifndef TREEWID_H
#define TREEWID_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QList>
#include "photo.h"
#include "db.h"
#include "mdiDS.h"
#include "mdi.h"

class treeWid :public QTreeWidget
{
public:
	treeWid(QWidget* ,db*,QTreeWidget*,mdiDS*,mdi*);
	~treeWid(void);

	void updateTree();
	void selectedItemChange();
private slots:;
	
private:
	void displayPhoto(QList<photo*>*);
	void displayFace(QList<face*>*);
	void buildTree();

	db* database;
	QTreeWidget* treeWidget;
	int size;
	QList<QTreeWidgetItem*>* itemList;
	mdiDS* mdids;
	mdi* mdiArea;
};
#endif
