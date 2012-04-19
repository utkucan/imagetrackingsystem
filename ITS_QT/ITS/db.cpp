#include "db.h"
#include <QSqlDatabase>

db::db(QObject *parent){
	if(openDB()){
		createTables();
		insertLabel("Ali");
	//	insertLabel("srcn");
		displayDatabase();
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

bool db::insertLabel(QString s)
{	QSqlQuery	query(database);
	 /*query.exec("SELECT * FROM Person");
         while(query.next())
         {
			  int id = query.value(0).toInt();
			  QString name = query.value(1).toString();
         }
		 return true;	*/
	const QString INSERT("INSERT INTO Person(Pid,name) VALUES(NULL,:name);");
	//QSqlQuery	query(database);
	 query.bindValue( ":name", s );		
	if(query.exec(INSERT))
	{
		qDebug() << "Table created";

		return true;
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
		return false;
	}/*
	const QString	SELECT("SELECT name FROM Person");
	if(query.exec(SELECT))
	{
        QString country = query.value(0).toInt();
		return true;
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
		return false;
	}*/
}
void db::createTables(){
	const QString	CREATE_TABLE("CREATE TABLE Images (Iid int AUTO_INCREMENT  not null PRIMARY KEY,path TEXT);");
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

	const QString	CREATE_TABLE2("CREATE TABLE Person (Pid int AUTO_INCREMENT not null PRIMARY KEY,name TEXT);");
	if(query.exec(CREATE_TABLE2))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}


	const QString	CREATE_TABLE3("CREATE TABLE Faces (Fid int AUTO_INCREMENT not null PRIMARY KEY,x double ,y double,width double,height double,tw double ,th double );");
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