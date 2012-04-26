#ifndef TREEWID_H
#define TREEWID_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QList>
#include "db.h"

class treeWid :public QTreeWidget
{
public:
	treeWid(QWidget* ,db*,QTreeWidget*);
	~treeWid(void);
	void selectedItemChange();
private slots:;
	
private:

	void buildTree();

	db* database;
	QTreeWidget* treeWidget;

	QList<QTreeWidgetItem*>* itemList;
	
};
#endif
