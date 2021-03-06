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
	void checkRecognizedFaces();
	bool getReDoFlag(){return reDoRunning;}
	bool getUpdateFaces(){return updateFaces;}
private slots:;
	
private:
	enum operation{photoOperation,faceOperation,SearchOperation};
	
	void displayPhoto(QList<int>*);
	void displayFace(QList<int>*,QList<int>*);
	
	void buildTree(QStringList personList);
	void buildSearchTree(QStringList personList);
	void buildLists();
	string QStringToString(QString str);

	int lastOperation;
	QString personName;
	QStringList SearchPersonList;
	bool reDoFlag;
	bool reDoRunning;
	bool updateFaces;
	int listPos;
	int size;

	QList<photo*> *photoList;
	QList<face*> *faceList;

	//ranking* r;
	db* database;
	QTreeWidget* treeWidget;
	QTreeWidgetItem* itemSearch;
	QTreeWidgetItem* itemPhotos;
	QList<QTreeWidgetItem*>* SearchItemList;
	QList<int> personNonApprovedFaceCount;
	QStringList personPosList;
	mdiDS* mdids;
	mdi* mdiArea;
//	QList<int>* preDisplayedFaces;
};
#endif
