#include "its.h"

ITS::ITS(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	mdiArea = new mdi(ui.mdiArea);
	faceList = new QList<face*>();
	imageAnalyzer = new analyzer(faceList);
	faceCount = 0;
}

ITS::~ITS()
{

}


void ITS::displayFace(){
	for(; faceCount< faceList->size(); faceCount++){
		mdiArea->addMdiChild((*faceList)[faceCount]);
	}
}

void ITS::on_actionKlasor_triggered(){
	QListWidgetItem *item;
    QCheckBox *cbox;
    
    QStringList QDirectory = QFileDialog::getOpenFileNames(
                         this,
                         "Select one or more files to open",
                         "",
                         "Images (*.jpg)");

	if (QDirectory.count() !=0) {
		for(int j = 0 ; j< QDirectory.count(); j++){

			string filename = QStringToString(QDirectory[j]);

			imageAnalyzer->analyz(filename,faceCount);
			displayFace();
		}
	}
//	QFileSystemModel *model = new QFileSystemModel;
//     model->setRootPath(QDir::currentPath());
 //    QTreeView *tree = new QTreeView(splitter);
//     ui.treeView->setModel(model);
//	 ui.treeView->m
}

string ITS::QStringToString(QString str){
	string filename ="";
	for(int i = 0; i<str.size();i++){
		char c = (str[i].toAscii());
		filename += c;
	}
	return filename;
}