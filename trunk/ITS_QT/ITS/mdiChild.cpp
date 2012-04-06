#include <QtGui>
#include "mdiChild.h"
#include <QDebug>

 MdiChild::MdiChild(QMdiArea* parent,face* FaceObject)
 {
	 prnt = parent;
	 h = 0;
     setAttribute(Qt::WA_DeleteOnClose);
	 this->FaceObject = FaceObject;
	 
	 

	 image = new QLabel(this);
	 image->setScaledContents(true);
//	 image->setAlignment(Qt::AlignTop);
	 image->setPixmap(*transformImage());
	 image->setGeometry(0,0,w,h);

	 if(FaceObject->getLabel() == ""){
		labelTextEdit = new QLineEdit(this);
//		labelTextEdit->setAlignment(Qt::AlignBottom);
		labelTextEdit->setGeometry(0,h,w,20);
		labelTextEdit->setText("add label");
		//labelTextEdit->addAction("add label");
		h +=20;
	 }
	 
	 this->setContextMenuPolicy(Qt::CustomContextMenu);
	 

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
	 
	 if(e->type() == QEvent::MouseButtonPress){
		 if( ((QMouseEvent*)e)->button() == Qt::RightButton){
			QPoint globalPos = this->mapToGlobal(((QMouseEvent*)e)->pos());
			QMenu myMenu(this);
			if(FaceObject->getLabel() == ""){
				myMenu.addAction("add label");
				QAction* selectedItem = myMenu.exec(globalPos);
				if(selectedItem->text() == "add label"){
					int a = 5;
				}
			}else{
				
			}
		 }
	 }

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

 QPixmap* MdiChild::transformImage(){
	 QImage* img = FaceObject->getImage();

	QPixmap* p = new QPixmap();
	p->convertFromImage(*img,Qt::AutoColor);
	this->w = FaceObject->getTransformedWidth();
	this->h += FaceObject->getTransformedHeight();
	return p;
 }

  void MdiChild::closeEvent(QCloseEvent *event)
 {
	 delete prnt;
	 prnt = NULL;
	 delete widgetadd;
	 widgetadd = NULL;
	 delete FaceObject;
	 FaceObject = NULL;	
	 delete e;
	 e = NULL;
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
