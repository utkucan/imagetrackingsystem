#include <QtGui>
#include "mdiChild.h"
#include <QDebug>

 MdiChild::MdiChild(QMdiArea* parent,face* FaceObject)
 {
	 prnt = parent;
     setAttribute(Qt::WA_DeleteOnClose);
	 setScaledContents(true);
	 setAlignment(Qt::AlignTop);
	 setAlignment(Qt::AlignLeft);
	 this->FaceObject = FaceObject;

	 setPixmap(*transformImage(FaceObject->getImage()));
 }

 void MdiChild::setChildGeometry(int posX,int posY){
	 this->posX = posX;
	 this->posY = posY;
	 widgetadd->setGeometry(QRect(posX,posY,w,h));
 }

 void MdiChild::setSubWinAdd(QMdiSubWindow* subwin){
	widgetadd = subwin;
 }

 bool MdiChild::event ( QEvent * e ){
	 
	 if(e->type() == QEvent::MouseButtonDblClick){
		 QImage *img = new QImage ( FaceObject->getPath().c_str() );
		 
		 popupDisplay* popup = new popupDisplay(img);
		 popup->show();



//		 QFrame* frm = new QFrame(this,Qt::Popup|Qt::Window);
//		 frm->setWindowState(Qt::WindowState::WindowFullScreen);
//		 popupDisplay* popup = new popupDisplay(img);
	//	 popup->setParent(frm);
		 
	//	 popup->show();
//		 frm->show();
	 }
	 return QWidget::event(e);
 
 }

 QPixmap* MdiChild::transformImage(QImage* img){
	QPixmap* p = new QPixmap();
		p->convertFromImage(*img,Qt::AutoColor);
	if(p->height() > 100 || p->width()>100){
		if(p->height()>p->width())
			p = (new QPixmap(p->scaledToHeight( 100,Qt::FastTransformation)));
		else
			p = (new QPixmap(p->scaledToWidth( 100,Qt::FastTransformation)));
	}
	this->w = p->width();
	this->h = p->height();
	return p;
 }

  void MdiChild::closeEvent(QCloseEvent *event)
 {
	prnt = NULL;
	widgetadd = NULL;

/*     if (maybeSave()) {
         event->accept();
     } else {
         event->ignore();
     }*/
 }







 bool MdiChild::loadFile(const QString &fileName)
 {
/*     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("MDI"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream in(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     setPlainText(in.readAll());
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);

     connect(document(), SIGNAL(contentsChanged()),
             this, SLOT(documentWasModified()));
*/
     return true;
 }

 bool MdiChild::save()
 {
/*     if (isUntitled) {
         return saveAs();
     } else {
         return saveFile(curFile);
     }*/
	 return true;
 }

 bool MdiChild::saveAs()
 {
 /*    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                     curFile);
     if (fileName.isEmpty())
         return false;

     return saveFile(fileName);*/
	 return true;
 }

 bool MdiChild::saveFile(const QString &fileName)
 {
 /*    QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("MDI"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << toPlainText();
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);*/
     return true;
 }

 QString MdiChild::userFriendlyCurrentFile()
 {
	 return strippedName(QString( FaceObject->getPath().c_str() ));
 }

 void MdiChild::documentWasModified()
 {
//     setWindowModified(document()->isModified());
 }

 bool MdiChild::maybeSave()
 {
 /*    if (document()->isModified()) {
         QMessageBox::StandardButton ret;
         ret = QMessageBox::warning(this, tr("MDI"),
                      tr("'%1' has been modified.\n"
                         "Do you want to save your changes?")
                      .arg(userFriendlyCurrentFile()),
                      QMessageBox::Save | QMessageBox::Discard
                      | QMessageBox::Cancel);
         if (ret == QMessageBox::Save)
             return save();
         else if (ret == QMessageBox::Cancel)
             return false;
     */
     return true;
 }

 void MdiChild::setCurrentFile(const QString &fileName)
 {
 /*    curFile = QFileInfo(fileName).canonicalFilePath();
     isUntitled = false;
     document()->setModified(false);
     setWindowModified(false);
     setWindowTitle(userFriendlyCurrentFile() + "[*]");*/
 }

 QString MdiChild::strippedName(const QString &fullFileName)
 {
     return QFileInfo(fullFileName).fileName();
 }
