#ifndef DB_H
#define DB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QDebug>
#include <QString>

#include "photo.h"
#include "face.h"
#include "displaydb.h"
#include "facedetect.h"

class db : public QObject
{
public:
	db(QObject *parent = 0);
	~db(void);

	bool insertPhoto(photo*);
	bool insertLabel(QString);
	void displayDatabase();
	
	QList<photo*>* getUnlabeledPhotos();
	bool updateHasFaces(int faceId, QString s, int imageId);
	bool insertIntoPerson(QString name);
	bool insertIntoPhoto(photo* p);
	bool insertIntoFaces(face* f, int);
	bool insertIntoHasFaces(int faceId, int personId, int imageId);
	QImage* IplImage2QImage(const IplImage *iplImage);
	string QStringToString(QString str);
	QSqlDatabase* getDB(){return &database;}
private:
	bool openDB();
	bool deleteDB();
	QSqlError lastError();
	void createTables();


	QSqlDatabase database;
};

#endif
//	Your Dijkstra Account Info:
 //Username: buyukgebiz
//Password: 75d11safi
