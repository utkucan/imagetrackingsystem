#ifndef DB_H
#define DB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QDebug>
#include <QString>

#include "Rank.h"
#include "photo.h"
#include "face.h"
#include "displaydb.h"


class db : public QObject
{
public:
	
	db(QObject *parent = 0);
	~db(void);
	int selectMaxFaceId();
	bool updateFaceforFeature(int, QString);
	QList<int>* selectAllFacesFromPersonId(int);
	bool insertPhoto(photo*);
	bool insertLabel(QString);
	bool insertIntoPerson(QString name);
	bool insertIntoPhoto(photo* p);
	bool insertIntoFaces(face* f, int);
	bool insertIntoHasFaces(int faceId, int personId, int imageId,int);
	bool insertIntoEqual(int, int,double);
	void storeFeatureList(double *);
	QList<Rank*> selectFromEqual(int); 
	QList<int>* selectPersonsInPhoto(QStringList );
	QList<int>* selectAllPerson();
	void getAllPhotos(QList<photo*>*,QList<face*>*);
	QList<int>* selectPersonPhoto(QString personName);
	QList<int>* selectPersonFace(QString personName,QList<int>*);
	bool photoExist(QString path);
	bool selectPerson();
	double* returnFacialFeatures(QString);
	
	QList<photo*>* getUnlabeledPhotos();

	QStringList getAllPerson();
	
	QSqlDatabase* getDB(){return &database;}
	int getPersonId(QString personName);
	QString getPersonName(int PersonID);
	QString getImagePath(int imageID);
	photo* getImage(int imageId);
	face* getFace(int faceId);
	QList<int>* getAllUnknownFaceIDs();
	bool updateHasFaces(int faceId, QString s, int imageId,int Approved);
	
	void InsertNonProcessedPhotoPaths(QStringList);
	void DeleteNonProcessedPhotoPaths(QString);
	QStringList GetNonProcessedPhotoPaths();
	QString GetFirstNonProcessedPhotoPaths();

	QList<int> getNonApprovedFace();
	QList<int>* selectLabelledPerson();
	int getLabelId(int);
private:
	string QStringToString(QString str);
	void displayDatabase();
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

