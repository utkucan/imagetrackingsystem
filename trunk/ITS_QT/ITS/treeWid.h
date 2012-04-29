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
	void reDoLastOperation();
	QList<face*>* getFaceList(){return faceList;}
	QList<photo*>* getPhotoList(){return photoList;}
private slots:;
	
private:
	enum operation{photoOperation,faceOperation,filterOperation};
	
	void displayPhoto(QList<int>*);
	void displayFace(QList<int>*,QList<int>*);
	
	void buildTree(QStringList personList);
	void buildFilterTree(QStringList personList);
	void buildLists();

	int lastOperation;
	QString personName;
	QStringList filterPersonList;
	bool reDoFlag;
	int listPos;
	int size;

	QList<photo*> *photoList;
	QList<face*> *faceList;

	db* database;
	QTreeWidget* treeWidget;
	QTreeWidgetItem* itemFilter;
	QTreeWidgetItem* itemPhotos;
	QList<QTreeWidgetItem*>* FilterItemList;
	mdiDS* mdids;
	mdi* mdiArea;
};
#endif
