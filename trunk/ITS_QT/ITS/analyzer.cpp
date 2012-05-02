#include "analyzer.h"


analyzer::analyzer(QList<face*> *faceList,string imagePath,matlab* m)
{	

	this->faceList = faceList;
	this->imagePath = imagePath;
	this->m = m;
	count = 1;
//	m = new matlab();
}

analyzer::~analyzer(void)
{
	faceList = NULL;
}

QImage* analyzer::transformImage(QImage* faceImage){
	 QImage* tmp;
	 if(faceImage->height()>faceImage->width()){
		tmp = new QImage(faceImage->scaledToHeight( 100,Qt::FastTransformation));
	 }else{
		tmp = new QImage(faceImage->scaledToWidth( 100,Qt::FastTransformation));
	 }
	 delete faceImage;
	 return tmp;
}

QImage* analyzer::IplImage2QImage(const IplImage *iplImage)
{
	int height = iplImage->height;
	int width = iplImage->width;
 
	if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
	{
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage* img = new QImage(qImageBuffer, width, height, QImage::Format_RGB888);
		img = new QImage(img->rgbSwapped());
		return img;
	} else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage* img = new QImage(qImageBuffer, width, height, QImage::Format_Indexed8);
 
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++){
			colorTable.push_back(qRgb(i, i, i));
		}
		img->setColorTable(colorTable);
		return img;
	}else{
		qWarning() << "Image cannot be converted.";
		return new QImage();
	}
}

void analyzer::run(){
 	facedetect f(imagePath);
	facedetect::List* head = f.getFaceList();
	facedetect::List* tmp = head;
	double* featureArray;
	while( tmp !=NULL){
		QImage* img = IplImage2QImage(tmp->face);
		img = transformImage(img);
//		saveface(img);
		img->save("temp.jpg");
		saveVJFile("temp.jpg",img);
		featureArray = m->returnFeatures("temp.jpg");
		
		// burada facial feature hesaplicaz, NULL yazan yere featurelar� verecez, label'� �imdilik bo� verdik, id'yi its set edicek
		
		face* fc = new face(0,imagePath,tmp->x,tmp->y,tmp->width,tmp->height,img->width(),img->height(),featureArray,"Unknown");
		faceList->append(fc);
		
		
		
		// asdkl�asdkl�kl�asdfjjk�asfla�sfklsfk�lafk�slff�asklsfkl�l�asfkkl�asf�klasf�asfkl�klasf�klasf�klasfkl�asfk�lasfkl�asfkl�
		/*ranking* r = new ranking(dbObj);
		for(int i = 0 ; i<faceList->size()-1;i++){
			r->compareFaces(fc,faceList->at(i));
		}*/
		
			
//		mdiArea->addMdiChild(fc);
		tmp= tmp->next;
	}
	f.clearList();
}


void analyzer::saveVJFile(QString path, QImage* img){
		QString s ;
		path = path.append(".vj");
		QByteArray ba = path.toLocal8Bit();
		const char *c_str2 = ba.data();

		ofstream myfile;
		myfile.open (c_str2);
		myfile << "1\n";

		s = QString("0 %1 0 %2").arg(img->width()).arg(img->height());
		//  s = "0 "+ img->width() + " 0 " + img->height() + "\n";
		ba = s.toLocal8Bit();
		c_str2 = ba.data();
		myfile << c_str2;
		myfile.close();
}
void  analyzer::saveface(QImage* image){
	char* no = new char[1];
	itoa(count,no,10);
	string out = "./face/";
	out+= (char)(*no);
	string imagename= "";
	int i = imagePath.length()-1; 
	while(imagePath[i] !='\\'){
		imagename = imagePath[i] + imagename;
		i--;
	}
	out += imagename;
	image->save(out.c_str());
	count++;
}

