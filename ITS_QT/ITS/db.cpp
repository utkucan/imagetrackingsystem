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

// bana face id verilcek ben butun tabledan faceidler olan faceidleri ve similarty doncem

QList<Rank*> db::selectFaceEqual(int faceId){
	bool a=false;
	QList<Rank*> myList ;
	QSqlQuery query(database);
	query.prepare("SELECT F2id, similar FROM Equal WHERE F1id= :face1 ");// union  SELECT F1id, similar FROM Equal WHERE F2id=:face1");//AND F2id IN (SELECT DISTINCT H.Fid FROM HasFaces H,Person P WHERE  P.Pid=H.Pid AND H.Approved=1  AND P.name NOT IN (SELECT P1.name FROM HasFaces H1,Person P1 WHERE  P1.Pid=H1.Pid  AND P1.name='Unknown'))");
		//union  SELECT F1id, similar FROM Equal WHERE F2id=:face1");
	query.bindValue(":face1", faceId);
	a=query.exec();

	while(query.next()){
		int fid=query.value(0).toInt();
	//	int lblId = getLabelId(fid);
		Rank *r= new Rank(fid, query.value(1).toDouble());

		myList.append(r);
	}

	return myList;
}

QList<int> db::getNonEqual(int fid){
	QSqlQuery query(database);
	query.prepare("SELECT P.Pid FROM Person P, NonEqual N WHERE N.Fid=:fid AND P.Pid=N.Pid");
	query.bindValue(":fid", fid);
	query.exec();
	QList<int> fids;
	while(query.next()){
		int nonEqulaPid = query.value(0).toInt();
		if(nonEqulaPid != 0)
			fids.append(nonEqulaPid);
	}
	return fids;
}

QList<Rank*> db::selectFromEqual(int faceId){
	//SELECT DISTINCT H.Fid FROM HasFaces H,Person P WHERE  P.Pid=H.Pid AND H.Approved=1  AND P.name NOT IN (SELECT P1.name FROM HasFaces H1,Person P1 WHERE  P1.Pid=H1.Pid  AND P1.name='Unknown') ");
	bool a=false;
	QList<Rank*> myList ;
	QSqlQuery query(database);
	query.prepare("SELECT F2id, similar FROM Equal WHERE F1id= :face1 AND F2id IN (SELECT DISTINCT H.Fid FROM HasFaces H,Person P WHERE  P.Pid=H.Pid AND H.Approved=1  AND P.name NOT IN (SELECT P1.name FROM HasFaces H1,Person P1 WHERE  P1.Pid=H1.Pid  AND P1.name='Unknown'))");
		//union  SELECT F1id, similar FROM Equal WHERE F2id=:face1");
	query.bindValue(":face1", faceId);
	a=query.exec();
	QList<int> NonEqPids = getNonEqual(faceId);
	while(query.next()){
		int fid=query.value(0).toInt();
		int lblId = getLabelId(fid);
		if(!NonEqPids.contains(lblId)){
			Rank *r= new Rank(lblId, query.value(1).toDouble());

			myList.append(r);
		}
	}

	return myList;

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
				p->getFaces()->at(i)->setPhotoID(k);
				insertIntoFaces(p->getFaces()->at(i),k);

			}


		}

		return a;
	}
}

bool db::insertIntoFaces(face* f,int photoId){
	{
		bool a=false;
		QSqlQuery query(database);
		query.prepare("INSERT INTO Faces VALUES(NULL , :x, :y , :width, :height , :tw , :th, :fet )");
		query.bindValue(":x", f->getX());
		query.bindValue(":y", f->getY());
		query.bindValue(":width", f->getWidth());
		query.bindValue(":height", f->getHeight());
		query.bindValue(":tw", f->getTransformedWidth());
		query.bindValue(":th", f->getTransformedHeight());

		double *fl=f->getFecialFeatures();
		QString featureStr = "";
		QString temp;
		char buffer [50];
		for(int i = 0 ; i<1937; i++){
			sprintf(buffer,"%f",fl[i]);
			featureStr.append(buffer);
			if(i!=1936){
				featureStr.append("#");
			}

		}
		query.bindValue(":fet",featureStr );


		a=query.exec();
		if(a){

			query.prepare("SELECT MAX(Fid) FROM Faces");
			a=query.exec();	
			int k= 0;
			query.next();
			k=query.value(0).toInt();
			f->setID(k);
			insertIntoHasFaces(k,1,photoId,0);
		}

		return a;
	}

}
//
bool db::insertIntoHasFaces(int faceId, int personId, int imageId,int Approved){
	{
		bool a=false;
		QSqlQuery query(database);
		query.prepare("INSERT INTO HasFaces VALUES( :fId , :pId , :IId, :A )");
		query.bindValue(":fId", faceId);
		query.bindValue(":pId",personId);
		query.bindValue(":IId", imageId);
		query.bindValue(":A", Approved);
		a=query.exec();
		return a;
	}


}

//2 faceid aliosun benzerlik orani onu sakliosun


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
	QString featStr = query.value(7).toString();
	double* featList = returnFacialFeatures(featStr);
	return new face(faceId,"",x,y,width,height,tw,th,featList,"");
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
QList<int>* db::selectPersonsInPhoto(QStringList personName){
	bool a=false;
	QSqlQuery query(database);
	QString s="";
	for(int i=0; i<personName.count(); i++){
		s=s+"SELECT I1.Iid FROM Images I1,HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND P1.Pid=H1.Pid AND H1.Iid=I1.Iid AND P1.name="+"'"+personName.at(i)+"'";
		if(i!=personName.count()-1)
			s=s+" INTERSECT ";
	}


	query.prepare(s);
	a=query.exec();
	QList<int>* integer = new QList<int>();
	while(query.next()){

		integer->append(query.value(0).toInt());

	}
	return integer;
}
bool db::deleteFromSuggested(int fid){
	QSqlQuery query(database);
	query.prepare("DELETE FROM Suggested WHERE Fid=:fid");
	query.bindValue(":fid", fid);
	return query.exec();
}

QStringList db::getSuggested(int fid){
	QSqlQuery query(database);
	query.prepare("Select P.name FROM Person P,Suggested S WHERE S.Fid=:fid AND S.Pid = P.Pid ORDER BY S.ratio ASC");
	query.bindValue(":fid", fid);
	query.exec();
	QStringList suggestedPersons;
	//suggestedPersons.append("");
	while(query.next()){
		suggestedPersons.append(query.value(0).toString());
	}
	return suggestedPersons;
}

QList<int>* db::selectPersonPhoto(QString personName){

	bool a=false;
	QSqlQuery query(database);
	if(personName != "Unknown")
		query.prepare("SELECT DISTINCT I.Iid FROM Faces F,HasFaces H,Images I,Person P WHERE H.Approved = 1 AND F.Fid=H.Fid AND P.Pid=H.Pid AND H.Iid=I.Iid AND I.Iid IN (SELECT I1.Iid FROM Images I1,HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND P1.Pid=H1.Pid AND H1.Iid=I1.Iid AND P1.name=:personName)");
	else
		query.prepare("SELECT DISTINCT I.Iid FROM Faces F,HasFaces H,Images I,Person P WHERE F.Fid=H.Fid AND P.Pid=H.Pid AND H.Iid=I.Iid AND I.Iid IN (SELECT I1.Iid FROM Images I1,HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND P1.Pid=H1.Pid AND H1.Iid=I1.Iid AND P1.name=:personName)");
	query.bindValue(":personName", personName);
	a=query.exec();
	QList<int>* photoIdList = new QList<int>();

	while(query.next()){
		photoIdList->append(query.value(0).toInt());
	}
	return photoIdList;

}
//utkucanin istedigi qquery



//emrenin istedigi person id yi verip butun face idlerini getirme hasface tablosunda
QList<int>* db::selectAllFacesFromPersonId(int personId){
	bool a=false;
	//	int personId= getPersonId(personName);

	QSqlQuery query(database);
	query.prepare("SELECT F1.Fid FROM HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND H1.Approved = 1 AND P1.Pid=H1.Pid AND P1.Pid=:personId");
	query.bindValue(":personId", personId);
	a=query.exec();
	QList<int>* faceIdList = new QList<int>();
	while(query.next()){
		faceIdList->append(query.value(0).toInt());
	}
	return faceIdList;
}
QList<int>* db::selectPersonFace(QString personName,QList<int>*ApprovedList){
	bool a=false;
	//	int personId= getPersonId(personName);

	QSqlQuery query(database);

	query.prepare("SELECT F1.Fid, H1.Approved FROM Images I1,HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND P1.Pid=H1.Pid AND H1.Iid=I1.Iid AND P1.name=:personName");
	query.bindValue(":personName", personName);
	a=query.exec();

	QList<int>* faceIdList = new QList<int>();
	//QList<int>* ApprovedList = new QList<int>();
	while(query.next()){
		faceIdList->append(query.value(0).toInt());
		ApprovedList->append(query.value(1).toInt());
	}
	return faceIdList;
}

bool db::photoExist(QString path){
	bool a=false;
	QSqlQuery query(database);
	query.prepare("SELECT COUNT(*) FROM Images WHERE path=:pathname");
	query.bindValue(":pathname",path );
	a=query.exec();
	if(a){
		query.next();
		int count = query.value(0).toInt();
		if(count == 0)
			return false;
		return true;
	}
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
				QString featureListStr = query3.value(7).toString();

				QSqlQuery query5(database);
				query5.prepare("SELECT name FROM Person WHERE Pid = :PId ");
				query5.bindValue(":PId", personId);
				bool k5=query5.exec();
				query5.next();
				QString lbl(query5.value(0).toString());

				double* featureList = returnFacialFeatures(featureListStr);

				face* f = new face(faceId,QStringToString(imagePath),x,y,width,height,tw,th,featureList,QStringToString(lbl));
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

double* db::returnFacialFeatures(QString str){
	QList<QString> fList = str.split("#");
	double* ff = new double[fList.size()];
	for(int p = 0 ; p<fList.size() ; p++ ){
		ff[p] = fList.at(p).toDouble();
	}
	return ff;
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

void db::getAllPhotos(QList<photo*>* pl,QList<face*>* faceList){
	bool a=false;
	QSqlQuery query(database);
	// "SELECT DISTINCT F.Fid,I.Iid,F.x,F.y,F.width,F.height,F.tw,F.th,I.path,P.name FROM Faces F,HasFaces H,Images I,Person P WHERE F.Fid=H.Fid AND P.Pid=H.Pid AND H.Iid=I.Iid AND I.Iid IN (SELECT I1.Iid FROM Images I1,HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND P1.Pid=H1.Pid AND H1.Iid=I1.Iid AND P1.name=:personName)");
	query.prepare("SELECT DISTINCT F.Fid,I.Iid,F.x,F.y,F.width,F.height,F.tw,F.th,I.path,P.name,F.feature FROM Faces F,HasFaces H,Images I,Person P WHERE F.Fid=H.Fid AND P.Pid=H.Pid AND H.Iid=I.Iid AND I.Iid IN (SELECT I1.Iid FROM Images I1,HasFaces H1,Faces F1,Person P1 WHERE F1.Fid=H1.Fid AND P1.Pid=H1.Pid AND H1.Iid=I1.Iid )");
	//	query.bindValue(":personName", personName);
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
		QString featStr = query.value(10).toString();
		if(!imageIdList.contains(imageId)){
			imageIdList.append(imageId);
			pathList.append(path);
		}
		double* flist = returnFacialFeatures(featStr);
		face* f = new face(faceId,QStringToString(path),x,y,width,height,tw,th,flist,QStringToString(name));
		f->setPhotoID(imageId);

		fl.append(f);
		faceList->append(f);
	}
	//	QList<photo*>* pl = new QList<photo*>();
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
}

bool db::insertIntoEqual(int face1, int face2, double sim){

	bool a=false;
	QSqlQuery query(database);
	query.prepare("INSERT INTO Equal VALUES(:fid1, :fid2 , :simi)");

	query.bindValue(":fid1", face1);
	query.bindValue(":fid2", face2);
	query.bindValue(":simi", sim);
	a=query.exec();
	return a;
}

bool db::updateHasFaces(int faceId, QString  s, int imageId,int Approved ){

	QSqlQuery query(database);
	query.prepare("SELECT Pid FROM Person WHERE name = :name1");
	query.bindValue(":name1",s );
	if(query.exec()){
		query.next();
		int personId=query.value(0).toInt();
		if(personId == 0 ){

			insertIntoPerson(s);
			bool k=false;
			query.prepare("SELECT MAX(Pid) FROM Person");
			if(query.exec()){
				query.next();
				personId=query.value(0).toInt();
			}else
				return false;
		}

		query.prepare( "UPDATE HasFaces SET Pid = :pid, Approved = :A  WHERE Fid = :fid  AND Iid= :iid" );
		query.bindValue(":fid", faceId);
		query.bindValue(":pid", personId);
		query.bindValue(":iid", imageId);
		query.bindValue(":A", Approved);
		if(query.exec())
			return true;
		return false;
	}else
		return false;

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


	const QString	CREATE_TABLE3("CREATE TABLE Faces (Fid  INTEGER PRIMARY KEY,x double ,y double,width double,height double,tw double ,th double, feature TEXT );");
	if(query.exec(CREATE_TABLE3))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}

	const QString	CREATE_TABLE4("CREATE TABLE HasFaces (Fid int not null ,Pid int not null,Iid int not null,Approved int not null ,PRIMARY KEY(Fid,Pid,Iid),FOREIGN KEY(Fid) REFERENCES Faces(Fid),FOREIGN KEY(Pid) REFERENCES Person(Pid),FOREIGN KEY(Iid) REFERENCES Images(Iid));");
	if(query.exec(CREATE_TABLE4))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}

	const QString	CREATE_TABLE5("CREATE TABLE Equal (F1id int  ,F2id int, similar double,PRIMARY KEY(F1id,F2id) ) ;");
	if(query.exec(CREATE_TABLE5))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}

	const QString	CREATE_TABLE6("CREATE TABLE NonProcessed (nid INTEGER PRIMARY KEY,path TEXT UNIQUE) ;");
	if(query.exec(CREATE_TABLE6))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}

	const QString	CREATE_TABLE7("CREATE TABLE NonEqual(Fid int,Pid int,PRIMARY KEY(Fid,Pid));");
	if(query.exec(CREATE_TABLE7))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}

	const QString	CREATE_TABLE8("CREATE TABLE Suggested(Fid int, Pid int, ratio double,PRIMARY KEY(Fid,Pid) );");
	if(query.exec(CREATE_TABLE8))
	{
		qDebug() << "Table created";
	}
	else
	{
		QSqlError e = query.lastError();
		qDebug() << "Failed to create table:" << query.lastError();
	}
}


void db::InsertNonProcessedPhotoPaths(QStringList lst){
	for(int i = 0; i<lst.size(); i++){
		QSqlQuery query(database);
		query.prepare("INSERT INTO NonProcessed VALUES(NULL,:path)");
		query.bindValue(":path", lst[i]);
		bool a = query.exec();
		int b = 5;
	}
}

void db::DeleteNonProcessedPhotoPaths(QString pth){
	QSqlQuery query(database);
	query.prepare("DELETE FROM NonProcessed WHERE path=:path");
	query.bindValue(":path", pth);
	query.exec();
}

QStringList db::GetNonProcessedPhotoPaths(){
	QSqlQuery query(database);
	query.prepare("SELECT * FROM NonProcessed");
	query.exec();
	QStringList lst;
	while(query.next()){
		lst.append(query.value(1).toString());
	}
	return lst;
}

QString db::GetFirstNonProcessedPhotoPaths(){
	QSqlQuery query(database);
	query.prepare("SELECT P.path FROM NonProcessed P WHERE P.nid IN(SELECT MIN(nid) FROM NonProcessed)");
	query.exec();
	query.next();
	QString s = query.value(0).toString();
	DeleteNonProcessedPhotoPaths(s);
	return s;
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
//butun personlar

QList<int>* db::selectAllPerson(){

	bool a=false;


	QSqlQuery query(database);

	query.prepare("SELECT Pid From Person");

	a=query.exec();

	QList<int>* personIdList = new QList<int>();

	while(query.next()){
		personIdList->append(query.value(0).toInt());

	}
	return personIdList;

}

QList<int>* db::getAllUnknownFaceIDs(){
	bool a=false;

	QSqlQuery query(database);

	query.prepare("Select Fid From HasFaces WHERE Approved=0");

	a=query.exec();

	QList<int>* unknownPersonIdList = new QList<int>();

	while(query.next()){
		unknownPersonIdList->append(query.value(0).toInt());
	}
	return unknownPersonIdList;

}

int db::selectMaxFaceId(){
	bool a=false;

	QSqlQuery query(database);
	query.prepare("SELECT MAX(Fid) FROM Faces");
	a=query.exec();
	query.next();
	int k=query.value(0).toInt();
	return k+1;

}
bool db::updateFaceforFeature(int id, QString s){

	bool a=false;

	QSqlQuery query(database);
	query.prepare("UPDATE Faces SET  feature= :fet WHERE Fid = :fid  ");
	query.bindValue(":fet", s);
	query.bindValue(":fid", id);
	a=query.exec();
	return a;
}

QList<int> db::getNonApprovedFace(){
	QSqlQuery query(database);
	query.prepare("SELECT H.Fid FROM HasFaces H WHERE H.Approved = 0");
	query.exec();
	QList<int> fid;
	while(query.next()){
		fid.append(query.value(0).toInt());
	}
	
	return fid;
}

QList<int>* db::selectLabelledPerson(){

	bool a=false;
	QSqlQuery query(database);

	query.prepare("SELECT DISTINCT H.Fid FROM HasFaces H,Person P WHERE  P.Pid=H.Pid AND H.Approved=1  AND P.name NOT IN (SELECT P1.name FROM HasFaces H1,Person P1 WHERE  P1.Pid=H1.Pid  AND P1.name='Unknown') ");
	
	a=query.exec();
	QList<int>* personIdList = new QList<int>();

	while(query.next()){
		personIdList->append(query.value(0).toInt());
	}

	return personIdList;

}




int db::getLabelId(int faceId){
	QSqlQuery query(database);
	query.prepare("SELECT P.Pid FROM Person P, HasFaces H, Faces F WHERE P.Pid = H.Pid AND H.Fid = F.Fid AND F.Fid = :faceId");
	query.bindValue(":faceId", faceId);
	query.exec();
	query.next();
	return query.value(0).toInt();
}

void db::insertNonEqual(int fid, QString lbl){
	int pid = getPersonId(lbl);
	bool a=false;
	QSqlQuery query(database);
	query.prepare("INSERT INTO NonEqual VALUES(:fid,:pid);");
	query.bindValue(":fid", fid);
	query.bindValue(":pid", pid);
	a=query.exec();

}
bool db::insertIntoSuggested(int faceId, int labelId, double ratio){
	//UPDATE HasFaces SET Pid = :pid, Approved = :A  WHERE Fid = :fid  AND Iid= :iid
	bool a=false;
	QSqlQuery query(database);
	query.prepare("INSERT INTO Suggested VALUES(:fid,:pid,:ratio);");//query.prepare("UPDATE Suggested SET ratio = :ratio WHERE Fid = :fid  AND Pid = :pid");
	query.bindValue(":fid", faceId);
	query.bindValue(":pid", labelId);
	query.bindValue(":ratio",ratio);
	a=query.exec();
	if(a == false){
		//QSqlQuery query(database);
		query.prepare("UPDATE Suggested SET ratio = :ratio WHERE Fid = :fid  AND Pid = :pid");//query.prepare("INSERT INTO Suggested VALUES(:fid,:pid,:ratio);");
		query.bindValue(":fid", faceId);
		query.bindValue(":pid", labelId);
		query.bindValue(":ratio",ratio);
		a=query.exec();
	}
	return a;

}

void db::DeletePersonHasNoFace(){
	QSqlQuery query(database);
	query.prepare("DELETE FROM Person WHERE Pid NOT IN ( SELECT DISTINCT Pid FROM HasFaces )");
	query.exec();
}

void db::numOfPersonsRecognizedFace(QList<QString>* lbls,QList<int>* counts){
	QSqlQuery query(database);
	query.prepare("Select P.name, COUNT(Fid) FROM HasFaces H, Person P WHERE P.Pid = H.Pid AND H.Approved = 0 GROUP BY H.Pid");
	query.exec();
	while(query.next()){
		lbls->append(query.value(0).toString());
		counts->append(query.value(1).toInt());
	}
}

/*

*/