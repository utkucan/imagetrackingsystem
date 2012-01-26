// This is the main DLL file.

#include "stdafx.h"

#include "ITS.h"

ITS::facedetect::facedetect(int counter)
{
	// the name of input file formed by "counter" parameter
	char buffer [10];
	itoa (counter,buffer,10);
	string infile = "imagedir";
	int i = 0;
	do{
		infile = infile + buffer[i];
		i++;
		counter = counter/10;
	}while(counter!= 0);
	infile = infile+".txt";

	/*  1)open input file
		2)read the line which is represent the address of image
		3)investigate and detect the faces in the image
		4)go to step 2 until reaching the end of file
		5)close the file
	*/
	ifstream openfile(infile.c_str());
	string imagename;
	if(openfile.is_open()){
		while(openfile.good()){
			getline(openfile,imagename);
			IplImage* originalImage = NULL; //delete etmeyi unutma
			string errmsg;
			if(readImage(imagename,originalImage,errmsg)){
				double scale;
				image = modifyImage(originalImage,scale);

				head = NULL;
				numOfFace = 0;
				detectFaces();

				markFacesOnOriginalImage(originalImage,scale);

				int length = imagename.length();
				string out ="";		
				for(int i = imagename.length()-1; i> 0; i--){
					if(imagename[i] != '\\')
						out = imagename[i] + out;
					else
						break;
				}
				string outFileName = "detectedface/"+out;
				if(!writeImage(originalImage,outFileName,errmsg)){
					// if image cannot be writen do....
				}
				clearList();
				cvReleaseImage(&originalImage);
				IplImage* tmp = image;
				cvReleaseImage(&tmp);
			}else{
				// if the image cannot be red...
			}
		}
		openfile.close();
	}
	//cvShowImage( "video", image );
}

bool ITS::facedetect::readImage(string imagename,IplImage* &img, string &errmsg)
{
	/*try to read the image from the given address
	  if reading image is unsuccessful, stop and return.
	  otherwise continue detection operation.
	*/
	try{
		img = cvLoadImage(imagename.c_str());//((std::string)imagename).c_str()); 
		if(!img){
			errmsg = "Can not load image " + imagename;
			return false;
		}
	}catch(exception e){
		errmsg = e.what();
		return false;
	}
	img->origin = 0; // make the image origin topLeft corner
	return true;
}

IplImage* ITS::facedetect::modifyImage(IplImage* img, double& scale){
	/* iþlemlerin hýzlý bir þekilde olmasý ve en verimli sonuçlarýn alýnmasý için 
	resim üzerinde bir takým oynamalar yapýlacak olan method burasý.
	þimdilik sadece scale ediyor, ileride ýþýk ve renk filtrelemesi de yapýlabilir*/
	scale = determineOptimalScaleFactor(img);
	return scaleImage(img,scale);
}

double ITS::facedetect::determineOptimalScaleFactor(IplImage* img){
	/* bu artýk biþiler yapcak güzel bir scale factor bulcak
		þimdilik büyük kenarý 400 yapacak þekilde bir deðer dönüyor
	*/
	if(img->width > img->height){
		return 400.0/img->width;
	}else{
		return 400.0/img->height;
	}
}

IplImage* ITS::facedetect::scaleImage(IplImage* img,double scale)
{
	/*scale the image with given scale factor*/
	IplImage* temp = cvCreateImage( cvSize( (int)((double)img->width)*scale ,(int)((double)img->height)*scale), img->depth, img->nChannels );
	cvResize(img, temp);
	return temp;
}

void ITS::facedetect::detectFaces()
{
	int cascadecase = 0;
	char *filename;
	while(cascadecase <=4){
		switch(cascadecase){
			case 0:
				filename = "haarcascade_profileface.xml";
				break;
			case 1:
				filename = "haarcascade_frontalface_alt.xml";
				break;
			case 2:
				filename = "haarcascade_frontalface_alt2.xml";
				break;
			case 3:
				filename = "haarcascade_frontalface_default.xml";
				break;
			case 4:
				filename = "haarcascade_frontalface_alt_tree.xml";
				break;
	
		}
		cascadecase++;
		operate(filename);
	}

}

void ITS::facedetect::operate(char *cascadeFileName)
{
	CvHaarClassifierCascade * cascade = ( CvHaarClassifierCascade* )cvLoad( cascadeFileName, 0, 0, 0 );
	CvMemStorage            *storage = cvCreateMemStorage( 0 );
	assert( cascade && storage);
 
	/* detect faces */
	CvSeq *faces = cvHaarDetectObjects(
			image,
			cascade,
			storage,
			1.1,
			3,
				CV_HAAR_DO_CANNY_PRUNING,
			cvSize( 40, 40 ) );
 
	/* for each face found, draw a red box */
	for( int i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
		CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );
		bool isexist = false;

		struct List* tmp =head;
		while(tmp !=NULL){
			int faceMidX = r->x + r->width/2;
			int faceMidY = r->y + r->height/2;
			if( (tmp->x < faceMidX) && 
				((tmp->x+tmp->width)>faceMidX) &&
				(tmp->y < faceMidY) &&
				((tmp->y+tmp->height)>faceMidY) ){
					int finX =tmp->x + tmp->width;
					int finY = tmp->y + tmp->height;

					if(r->x < tmp->x)
						tmp->x = r->x;
					if(r->y < tmp->y)
						tmp->y = r->y;
					if( r->x+r->width > finX)
						tmp->width =  r->x + r->width - tmp->x ;
					if(r->y+r->height > finY)
						tmp->height = r->y+r->height - tmp->y;
					isexist = true;
					break;
			}
			tmp = tmp->next;
		}
			
		if(!isexist){
			struct List* tmpList = new struct List();
			tmpList->x = r->x;
			tmpList->y = r->y;
			tmpList->width = r->width;
			tmpList->height = r->height;
			tmpList->next = NULL;

			addList(tmpList);
			numOfFace++;
		}
	}
	cvReleaseHaarClassifierCascade( &cascade );
	cvReleaseMemStorage( &storage );
}

void ITS::facedetect::addList(struct List* node){
			if(head == NULL){
				head = node;
			}else{
			//	struct List* node = new struct List();
				struct List* t = head;
				while(t->next != NULL){
					t = t->next;
				}
				t->next = node;
			}
		}

void ITS::facedetect::clearList(){
	struct List* tmp = head;
	while(tmp!=NULL){
		struct List* t = tmp;
		tmp = tmp->next;
		delete t;
	}
}

void ITS::facedetect::markFacesOnOriginalImage(IplImage* &img,double scale){
	struct List* tmp = head;
	for(int i = 0; i<numOfFace && tmp !=NULL; i++){
		cvRectangle( img,
					cvPoint( (int)((double)tmp->x / scale), (int)((double)tmp->y / scale) ),
					cvPoint( (int)((double)(tmp->x + tmp->width)/ scale), (int)((double)(tmp->y + tmp->height)/ scale) ),
					CV_RGB( 255, 0, 0 ), 3, 8, 0 );
		tmp= tmp->next;
	}
}

bool ITS::facedetect::writeImage(IplImage* img,string outFile,string& errmsg){
	if(!cvSaveImage(outFile.c_str(),img)){ 
		errmsg = "Could not save" +  outFile;
		return false;
	}
	return true;
}