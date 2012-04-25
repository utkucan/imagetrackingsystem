#include "db.h"
#include <QSqlDatabase>
#include "photo.h"
db::db(QObject *parent){
	if(openDB()){
		createTables();
		insertIntoPerson("Unknown");
	
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

		}

		for(int i=0; i < p->getFaces()->count()  ;i++ ){
			
			insertIntoFaces(p->getFaces()->at(i),k);

		}
		return a;
	}
}
bool db::updateHasFaces(int faceId, int personId, int imageId ){
	bool a=false;
	QSqlQuery query(database);
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
	const QString	CREATE_TABLE("CREATE TABLE Images (Iid  INTEGER PRIMARY KEY,path TEXT);");
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

void db::displayDatabase(){
	displaydb* disp = new displaydb();
	disp->show();
}