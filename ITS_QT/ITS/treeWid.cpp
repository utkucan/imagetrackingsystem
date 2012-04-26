#include "treeWid.h"


treeWid::treeWid(QWidget* prnt,db* database,QTreeWidget* treeWidge)
{
	this->database = database;
	this->treeWidget = treeWidge;

	itemList = new QList<QTreeWidgetItem*>();

	buildTree();

	/*
	for( int i = 0; i<QDirectory.size(); i++){
		
		QTreeWidgetItem *subItem = new QTreeWidgetItem();
		photoItem->addChild(subItem);
		QString imageName = QFileInfo(QDirectory[i]).fileName();
		subItem->setText(0,imageName);
	}
	*/
}


treeWid::~treeWid(void)
{
}


void treeWid::buildTree(){
	QStringList personList = database->getAllPerson();
	for( int i = 0; i<personList.size();i++){
		QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
		item->setText(0,personList[i]);

		itemList->append(item);
	}
}

void treeWid::selectedItemChange(){
	int a = 5;
}