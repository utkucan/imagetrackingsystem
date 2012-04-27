#include "db.h"
#include <QSqlDatabase>
#include "photo.h"
db::db(QObject *parent){
	if(openDB()){

		QSqlQuery query(database);
		bool a = query.exec("PRAGMA cache_size = 10000;");

		createTables();
		insertIntoPerson("Unknown");
	//	getUnlabeledPhotos();
	}

}
db::~db(void){
	deleteDB();
}
		  
bool db::openDB()
    {
    // Find QSLite driver
    database = QSqlDatabase::addDatabase("QSQLITE");
 
    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    database.setDatabaseName("./db/my.db.sqlite");
    #endif
 
    // Open databasee
	if(database.open())
		return true;
	return false;
//    return database.open();
    }
 
QSqlError db::lastError()
    {
    // If opening database has failed user can ask 
    // error description by QSqlError::text()
    return database.lastError();
    }
 
bool db::deleteDB()
    {
    // Close database
    database.close();
 
    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else
 
    // Remove created database binary file
    return QFile::remove("my.db.sqlite");
    #endif
    }

bool db::insertIntoPerson(QString name){
	{
		bool a=false;
		QSqlQuery query(database);
		query.prepare("INSERT INTO Person VALUES(NULL , :name)");
		query.bindValue(":name", name);
		a=query.exec();


		return a;
	}
}

bool db::insertIntoPhoto(photo *p){
	{
		bool a=false;
		QSqlQuery query(database);
		query.prepare("INSERT INTO Images VALUES(NULL , :name)");
		QString s=  QString(p->getPath().c_str());
		query.bindValue(":name", s);
		a=query.exec();
		int k=0;
		if(a){

			query.prepare("SELECT MAX(Iid) FROM Images");
			a=query.exec();	
			
			query.next();
			k=query.value(0).toInt();
			p->setID(k);

			for(int i=0; i < p->getFaces()->count()  ;i++ ){
			
				insertIntoFaces(p->getFaces()->at(i),k);

			}

		}

		return a;
	}
}

int db::getPersonId(QString personName){
	QSqlQuery query(database);
	query.prepare("SELECT  Pid FROM Person WHERE name = :personName ");
	query.bindValue(":personName",personName);
	bool a = query.exec();
	query.next();
	return query.value(0).toInt();
}

QString db::getPersonName(int PersonID){
	QSqlQuery query(database);
	query.prepare("SELECT  name FROM Person WHERE Pid = :PId ");
	query.bindValue(":PId",PersonID);
	bool a = query.exec();
	query.next();
	return query.value(0).toString();
}

QString db::getImagePath(int imageID){
	QSqlQuery query(database);
	query.prepare("SELECT path FROM Images WHERE Iid = :imageId ");
	query.bindValue(":imageId", imageID);
	bool k = query.exec();
	query.next();
	return query.value(0).toString(); 
}

face* db::getFace(int faceId){
	QSqlQuery query(database);
	query.prepare("SELECT * FROM Faces WHERE Fid = :FId ");
	query.bindValue(":FId", faceId);
	bool k=query.exec();
	query.next();

	double x=query.value(1).toDouble();
	double y=query.value(2).toDouble();
	double width=query.value(3).toDouble();
	double height=query.value(4).toDouble();
	double tw=query.value(5).toDouble(); 
	double th=query.value(6).toDouble();

	return new face(faceId,"",x,y,width,height,tw,th,NULL,"");
}

photo* db::getImage(int imageId){
	QSqlQuery query(database);
	query.prepare("SELECT DISTINCT Fid,Pid FROM HasFaces WHERE Iid = :imageId ");
	query.bindValue(":imageId", imageId);
	bool k=query.exec();
	QList<face*>* fl = new QList<face*>();
	QString imagePath = getImagePath(imageId);
	string path = QStringToString(imagePath);
	while(query.next()){
		int faceId = query.value(0).toInt(); 
		int personId = query.value(1).toInt(); 	

		face* f = getFace(faceId);

		f->setLabel(QStringToString(getPersonName(personId)));
		f->setPhotoID(imageId);
		f->setPath(path);
		fl->append(f);
	}
	photo* p = new photo(imagePath,fl);
	p->setID(imageId);
	return p;
}

QList<photo*>* db::selectPersonPhoto(QString personName){
	
	bool a=false;
	QSqlQuery query(database);

	query.prepare("SELECT DISTINCT F.Fid,I.Iid,F.x,F.y,F.width,F.height,F.tw,F.th,I.path,P.name FROM Faces F,HasFaces H,Images I,Person P WHERE F.Fid=H.Fid AND P.Pid=H.Pid AND H.Iid=I.Iid AND I.Iid IN (SELECT I1.Iid FROM Images I1,HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND P1.Pid=H1.Pid AND H1.Iid=I1.Iid AND P1.name=:personName)");
	query.bindValue(":personName", personName);
	a=query.exec();

	QList<face*> fl;
	QList<int> imageIdList;
	QList<QString> pathList;

	while(query.next()){
		int faceId = query.value(0).toInt();
		int imageId = query.value(1).toInt();
		double x=query.value(2).toDouble();
		double y=query.value(3).toDouble();
		double width=query.value(4).toDouble();
		double height=query.value(5).toDouble();
		double tw=query.value(6).toDouble(); 
		double th=query.value(7).toDouble();
		QString path = query.value(8).toString();
		QString name = query.value(9).toString();

		if(!imageIdList.contains(imageId)){
			imageIdList.append(imageId);
			pathList.append(path);
		}
		
		
		
		face* f = new face(faceId,QStringToString(path),x,y,width,height,tw,th,NULL,QStringToString(name));
		f->setPhotoID(imageId);

		
		fl.append(f);
	}
	QList<photo*>* pl = new QList<photo*>();
	for(int i = 0; i<imageIdList.size();i++){
		QList<face*> *pfl = new QList<face*>(); 
		for(int j = 0; j<fl.size();j++){
			if(fl[j]->getPhotoId() == imageIdList[i]){
				pfl->append(fl[j]);
				fl.removeAt(j);
				j--;
			}
		}
		photo* np = new photo(pathList[i],pfl);
		np->setID(imageIdList[i]);
		pl->append(np);
	}
	
	
	/*
	bool a=false;
	int personId= getPersonId(personName);

	QSqlQuery query(database);
	query.prepare("SELECT DISTINCT Iid FROM HasFaces WHERE Pid = :personId ");
	query.bindValue(":personId", personId);
	a=query.exec();

	QList<photo*>* pl = new QList<photo*>();
	while(query.next()){
		int imageId=query.value(0).toInt();
		pl->append(getImage(imageId));
	}
	*/
	return pl;
	
}

QList<face*>* db::selectPersonFace(QString personName){
	bool a=false;
	int personId= getPersonId(personName);

	QSqlQuery query(database);
	query.prepare("SELECT DISTINCT Fid,Iid FROM HasFaces WHERE Pid = :personId ");
	query.bindValue(":personId", personId);
	a=query.exec();

	QList<face*>* fl = new QList<face*>();
	while(query.next()){
		int faceId=query.value(0).toInt();
		int imageId=query.value(1).toInt();
		face* f = getFace(faceId);

		QString imagePath = getImagePath(imageId);
		string path = QStringToString(imagePath);
		f->setLabel(QStringToString(getPersonName(personId)));
		f->setPhotoID(imageId);

		
		f->setPath(path);

		fl->append(f);
	//	fl->append(getImage(imageId));
	}
	return fl;
}

QList<photo*>* db::getUnlabeledPhotos(){

	bool a=false;
	QSqlQuery query(database);
	query.prepare("SELECT DISTINCT Iid FROM HasFaces WHERE Pid = 1 ");
	a=query.exec();
//	QList<int> photoId;
	QList<photo*>* pl = new QList<photo*>();
	while(query.next()){
		int imageId=query.value(0).toInt();
//		photoId.append(imageId);
		QList<face*>* fl = new QList<face*>();

		QSqlQuery query4(database);
		query4.prepare("SELECT DISTINCT path FROM Images WHERE Iid = :imageId ");
		query4.bindValue(":imageId", imageId);
		bool k4 = query4.exec();
		query4.next();
		QString imagePath(query4.value(0).toString()); 


		QSqlQuery query2(database);
		query2.prepare("SELECT DISTINCT Fid,Pid FROM HasFaces WHERE Iid = :imageId ");
		query2.bindValue(":imageId", imageId);
		bool k=query2.exec();
		while(query2.next()){
			int faceId = query2.value(0).toInt(); 
			int personId = query2.value(1).toInt(); 
			QSqlQuery query3(database);
			query3.prepare("SELECT * FROM Faces WHERE Fid = :FId ");
			query3.bindValue(":FId", faceId);
			bool k3=query3.exec();
			//facelist olcak
			while(query3.next()){
				 double x=query3.value(1).toDouble();
				 double y=query3.value(2).toDouble();
				 double width=query3.value(3).toDouble();
				 double height=query3.value(4).toDouble();
				 double tw=query3.value(5).toDouble(); 
				 double th=query3.value(6).toDouble();

				

				QSqlQuery query5(database);
				query5.prepare("SELECT name FROM Person WHERE Pid = :PId ");
				query5.bindValue(":PId", personId);
				bool k5=query5.exec();
				 query5.next();
				 QString lbl(query5.value(0).toString());


				 face* f = new face(faceId,QStringToString(imagePath),x,y,width,height,tw,th,NULL,QStringToString(lbl));
				 f->setPhotoID(imageId);
				 fl->append(f);
			}
		}
		photo* p = new photo(imagePath,fl);
		p->setID(imageId);
		pl->append(p);
	}
	

	return pl;
}

bool db::updateHasFaces(int faceId, QString  s, int imageId ){
	bool a=false;
	QSqlQuery query(database);
	query.prepare("SELECT Pid FROM Person WHERE name = :name1");
	query.bindValue(":name1",s );
	a=query.exec();
	int personId=0;
	query.next();
	personId=query.value(0).toInt();
	if(personId == 0 ){
		
		insertIntoPerson(s);
		bool k=false;
		query.prepare("SELECT MAX(Pid) FROM Person");
		k=query.exec();	
		query.next();
		personId=query.value(0).toInt();
	}
	

	a=false;
	
	query.prepare( "UPDATE HasFaces SET Pid = :pid   WHERE Fid = :fid  AND Iid= :iid" );
	query.bindValue(":fid", faceId);
	query.bindValue(":pid", personId);
	query.bindValue(":iid", imageId);
	a=query.exec();
	return a;

}

bool db::insertIntoFaces(face* f, int photoId){
	{
		bool a=false;
		QSqlQuery query(database);
		query.prepare("INSERT INTO Faces VALUES(NULL , :x, :y , :width, :height , :tw , :th )");
		query.bindValue(":x", f->getX());
		query.bindValue(":y", f->getY());
		query.bindValue(":width", f->getWidth());
		query.bindValue(":height", f->getHeight());
		query.bindValue(":tw", f->getTransformedWidth());
		query.bindValue(":th", f->getTransformedHeight());
		a=query.exec();
		if(a){

			query.prepare("SELECT MAX(Fid) FROM Faces");
			a=query.exec();	
			int k= 0;
			query.next();
			k=query.value(0).toInt();
			f->setID(k);
	

			insertIntoHasFaces(k,1,photoId);
		}

		return a;
	}

}

bool db::insertIntoHasFaces(int faceId, int personId, int imageId){
	{
		bool a=false;
		QSqlQuery query(database);
		query.prepare("INSERT INTO HasFaces VALUES( :fId , :pId , :IId )");
		query.bindValue(":fId", faceId);
		query.bindValue(":pId",personId);
		query.bindValue(":IId", imageId);
		a=query.exec();
		return a;
	}


}

bool db::insertLabel(QString s)
{
	{
		bool a=false;
		QSqlQuery query(database);
		query.prepare("INSERT INTO Person VALUES(NULL , :name)");
	
		query.bindValue(":name", "aASDSsadak");
		 a=query.exec();

		query.prepare("INSERT INTO Person VALUES(NULL , :name)");
		query.bindValue(":name", "BURAKk");
		a=query.exec();
		query.prepare("INSERT INTO Person VALUES(NULL , :name)");
		query.bindValue(":name", "aadak");
		 a=query.exec();
		query.prepare("INSERT INTO Person VALUES(NULL , :name)");
		query.bindValue(":name", "aak");
		a=query.exec();

		query.prepare("INSERT INTO Images VALUES(NULL , :name)");
		query.bindValue(":name", "Image1");
		a=query.exec();
		query.prepare("INSERT INTO Images VALUES(NULL , :name)");
		query.bindValue(":name", "Image2");
		a=query.exec();


		query.prepare("INSERT INTO Faces VALUES(NULL , 10, 11 , 12, 13, 14,15 )");

		a=query.exec();

		query.prepare("INSERT INTO Faces VALUES(NULL , 12, 911 , 912, 193, 14.25 ,15 )");

		a=query.exec();

		query.prepare("INSERT INTO HasFaces VALUES(1,1,1 )");

		a=query.exec();
		  {
    // SELECT1

			QSqlQuery query(database);
			bool k=false;
			k=query.exec("SELECT Pid FROM Person");


			int x=-10;
			while (query.next()) {
				//QString name = query.value(0).toString();
				 x=query.value(0).toInt();
	
			}
			 {
    // SELECT1

			QSqlQuery query(database);
			bool k=false;
			k=query.exec("SELECT Iid FROM Images");


			int x=-10;
			while (query.next()) {
				//QString name = query.value(0).toString();
				 x=query.value(0).toInt();
	
			}

    }
    }
		return a;
	}
	
	
}

void db::createTables(){
	const QString	CREATE_TABLE("CREATE TABLE Images (Iid  INTEGER PRIMARY KEY,path TEXT UNIQUE);");
	QSqlQuery	query(database);
			
	if(query.exec(CREATE_TABLE))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}

	const QString	CREATE_TABLE2("CREATE TABLE Person (Pid INTEGER PRIMARY KEY,name TEXT UNIQUE );");
	if(query.exec(CREATE_TABLE2))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}


	const QString	CREATE_TABLE3("CREATE TABLE Faces (Fid  INTEGER PRIMARY KEY,x double ,y double,width double,height double,tw double ,th double );");
	if(query.exec(CREATE_TABLE3))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}

	const QString	CREATE_TABLE4("CREATE TABLE HasFaces (Fid int not null ,Pid int not null,Iid int not null ,PRIMARY KEY(Fid,Pid,Iid),FOREIGN KEY(Fid) REFERENCES Faces(Fid),FOREIGN KEY(Pid) REFERENCES Person(Pid),FOREIGN KEY(Iid) REFERENCES Images(Iid));");
	if(query.exec(CREATE_TABLE4))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}
}

QStringList db::getAllPerson(){
	bool a=false;
	QSqlQuery query(database);
	query.prepare("SELECT name FROM Person");
	a=query.exec();
	QStringList personList;
	while(query.next()){
		personList.append(query.value(0).toString());
	}
	return personList;
}


string db::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}


void db::displayDatabase(){
	displaydb* disp = new displaydb();
	disp->show();
}