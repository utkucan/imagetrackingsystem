#include "facedetect.h"

facedetect::facedetect(string fileName)
{
	//string infile = generateFileName(id);
	//string* fileNames = getImageName(infile,lenght);
//	for(int i = 0; i<lenght;i++){
		IplImage* originalImage = NULL;
		image = NULL;
		string errmsg; // if an error occurs in somewhere, it contains the error message
		if(readImage(fileName,originalImage,errmsg)){ // read image
			
			startDetection(originalImage);
			extractFaceImage(originalImage,scl);
			//writeFacesImage(originalImage,fileNames[i],errmsg,scl);
			/* write the image to the specified directory */
			/*if( !writeImage(originalImage,fileNames[i],errmsg)){ //!writeImage(originalImage,imagename,errmsg
				// if image cannot be writen do....
			}
				*/
	//		clearList(); // clear the link list
			IplImage* tmp = image; 
			cvReleaseImage(&tmp); // release the memory allocated for modified image
			cvReleaseImage(&originalImage); // release the memory allocated for original image
		}else{
			// if the image cannot be red...
		}
//	}
//	ifstream openfile(infile.c_str());
//	string imagename;
}

struct facedetect::List* facedetect::getFaceList(){

	return head;
}

double facedetect::getScale(){
	return scl;
}

void facedetect::clearList(){
	struct List* tmp = head;
	while(tmp!=NULL){
		struct List* t = tmp;
		tmp = tmp->next;
		cvReleaseImage(&t->face);
		t->next = NULL;
		delete t;
	}
}

bool facedetect::readImage(string imagename,IplImage* &img, string &errmsg)
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

void facedetect::startDetection(IplImage* &img){
	double scale = 1; // scale factor of image
	image  = modifyImage(img,scale); // modifies the image in order to get faster and more reliable results
	scl = scale;
	/*
	string errmsg;
	writeImage(image,"\\deneme.jpg",errmsg);
	*/
	head = NULL;
	numOfFace = 0;
	detectFaces(); // detects faces in the images
	//markFacesOnOriginalImage(img,scale); //draws rectangular frame around the detected faces
	
}

IplImage* facedetect::modifyImage(IplImage* img, double& scale){
	/* iþlemlerin hýzlý bir þekilde olmasý ve en verimli sonuçlarýn alýnmasý için 
	resim üzerinde bir takým oynamalar yapýlacak olan method burasý.
	þimdilik sadece scale ediyor, ileride ýþýk ve renk filtrelemesi de yapýlabilir*/
	scale = determineOptimalScaleFactor(img);
	IplImage* modified = scaleImage(img,scale);
	//illuminationCorrection(modified);
	//histogramEqualizedImage(modified);
	return modified;
}

double facedetect::determineOptimalScaleFactor(IplImage* img){
	/* bu artýk biþiler yapcak güzel bir scale factor bulcak
		þimdilik büyük kenarý 400 yapacak þekilde bir deðer dönüyor
	*/
	if(img->width > img->height){
		return 400.0/img->width;
	}else{
		return 400.0/img->height;
	}
}

IplImage* facedetect::scaleImage(IplImage* img,double scale)
{
	/*scale the image with given scale factor*/
	IplImage* temp = cvCreateImage( cvSize( (int)((double)img->width)*scale ,(int)((double)img->height)*scale), img->depth, img->nChannels );
	cvResize(img, temp);
	return temp;
}

void facedetect::detectFaces()
{
	operate(haarcascade_profileface);
	operate(haarcascade_frontalface_default);
/*	int cascadecase = 0;
	while(cascadecase <=4){
		operate(cascadecase);
		cascadecase++;
		
	}*/

}

char* facedetect::getHaarFileName(int id){
	switch(id){
		case haarcascade_profileface:
			return "haarcascade_profileface.xml";
		case haarcascade_frontalface_alt:
			return "haarcascade_frontalface_alt.xml";
		case haarcascade_frontalface_alt2:
			return "haarcascade_frontalface_alt2.xml";
		case haarcascade_frontalface_default:
			return "haarcascade_frontalface_default.xml";
		case haarcascade_frontalface_alt_tree:
			return "haarcascade_frontalface_alt_tree.xml";
		case haarcascade_eye:
			return "haarcascade_eye.xml";
		case haarcascade_eye_tree_eyeglasses:
			return "haarcascade_eye_tree_eyeglasses.xml";
		case haarcascade_fullbody:
			return "haarcascade_fullbody.xml";
		case haarcascade_lefteye_2splits:
			return "haarcascade_lefteye_2splits.xml";
		case haarcascade_lowerbody:
			return "haarcascade_lowerbody.xml";
		case haarcascade_mcs_eyepair_big:
			return "haarcascade_mcs_eyepair_big.xml";
		case haarcascade_mcs_eyepair_small:
			return "haarcascade_mcs_eyepair_small.xml";
		case haarcascade_mcs_lefteye:
			return "haarcascade_mcs_lefteye.xml";
		case haarcascade_mcs_mouth:
			return "haarcascade_mcs_mouth.xml";
		case haarcascade_mcs_nose:
			return "haarcascade_mcs_nose.xml";
		case haarcascade_mcs_righteye:
			return "haarcascade_mcs_righteye.xml";
		case haarcascade_mcs_upperbody:
			return "haarcascade_mcs_upperbody.xml";
		case haarcascade_righteye_2splits:
			return "haarcascade_righteye_2splits.xml";
		case haarcascade_upperbody:
			return "haarcascade_upperbody.xml";
	}
}

void facedetect::operate(int cascadeFileNameId)
{
	CvHaarClassifierCascade * cascade = ( CvHaarClassifierCascade* )cvLoad( getHaarFileName(cascadeFileNameId), 0, 0, 0 );
	CvMemStorage            *storage = cvCreateMemStorage( 0 );
	assert( cascade && storage);
 
	/* detect faces */
	CvSeq *faces = cvHaarDetectObjects(
			image,
			cascade,
			storage,	
			1.3,
			2,
			CV_HAAR_DO_CANNY_PRUNING,
			cvSize( 20, 20 ) );
 
	/* for each face found, draw a red box */
	for( int i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
		CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );
		bool isexist = false;
/*
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
			*/
		if(!isexist){
			struct List* tmpList = new struct List();
			tmpList->type = cascadeFileNameId;
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

void facedetect::addList(struct List* node){
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



void facedetect::markFacesOnOriginalImage(IplImage* &img,double scale){
	struct List* tmp = head;
	for(int i = 0; i<numOfFace && tmp !=NULL; i++){
		if(tmp->type == haarcascade_profileface)
			cvRectangle( img,
					cvPoint( (int)((double)tmp->x / scale), (int)((double)tmp->y / scale) ),
					cvPoint( (int)((double)(tmp->x + tmp->width)/ scale), (int)((double)(tmp->y + tmp->height)/ scale) ),
					CV_RGB( 255, 0, 0 ), 3, 8, 0 );
		else
			cvRectangle( img,
					cvPoint( (int)((double)tmp->x / scale), (int)((double)tmp->y / scale) ),
					cvPoint( (int)((double)(tmp->x + tmp->width)/ scale), (int)((double)(tmp->y + tmp->height)/ scale) ),
					CV_RGB( 0, 255, 0 ), 3, 8, 0 );
		tmp= tmp->next;
	}
}

void facedetect::extractFaceImage(IplImage* &img,double scale){
	struct List* tmp = head;

	for(int i = 0; i<numOfFace && tmp !=NULL; i++){
		IplImage* temp = cvCreateImage( cvSize( (int)((double)tmp->width / scale) ,(int)((double)tmp->height / scale)), img->depth, img->nChannels );
		cvSetImageROI(img,cvRect( (int)((double)tmp->x / scale), (int)((double)tmp->y / scale), (int)((double)tmp->width / scale) ,(int)((double)tmp->height / scale)));
		cvCopy( img, temp ); 
		cvResetImageROI( img );
		tmp->x =  (int)((double)tmp->x / scale);
		tmp->y =  (int)((double)tmp->y / scale);
		tmp->width = (int)((double)tmp->width / scale);
		tmp->height = (int)((double)tmp->height / scale);
		tmp->face = temp;
		tmp= tmp->next;
	}
}

void facedetect::writeFacesImage(IplImage* img,string imagename,string& errmsg,double scale){
	struct List* tmp = head;

	for(int i = 0; i<numOfFace && tmp !=NULL; i++){
		IplImage* temp = cvCreateImage( cvSize( (int)((double)tmp->width / scale) ,(int)((double)tmp->height / scale)), img->depth, img->nChannels );
		cvSetImageROI(img,cvRect( (int)((double)tmp->x / scale), (int)((double)tmp->y / scale), (int)((double)tmp->width / scale) ,(int)((double)tmp->height / scale)));
		cvCopy( img, temp ); 
		cvResetImageROI( img );
/*		char* tempdata= temp->imageData;
		char* data= img->imageData;

		int tmpX = (int)((double)tmp->x / scale);
		int tmpY = (int)((double)tmp->y / scale);
		int tmpH = (int)((double)tmp->height / scale);
		int tmpW = (int)((double)tmp->width / scale);
		int x = ((tmpY-1)*img->width + tmpX)*img->nChannels;
		int k = 0;
		for(;x<=((tmpY+tmpH-1)*img->width +tmpX+tmpW)*img->nChannels; x = x+img->width*img->nChannels){
			for(int y = x;y <= x+tmpW*img->nChannels;y++){
				tempdata[k] = data[y];
				k++;
			}
		}*/

	/*	int x = (tmp->y-1)*img->width*img->nChannels ;
		int ctr = 0;
		for(int j = 0 ; j < tmp->height ; j++){
			x = x + tmp->x*img->nChannels ;
			for(int k = 0 ; k < tmp->width*img->nChannels ; k++){
				tempdata[ctr++] = data[x+k];
			}
			x =  (tmp->y+j)*img->width*img->nChannels;
		}*/
		/*for(int j = ((tmp->y-1)*img->width + tmp->x)*img->nChannels , k = 0;
				j< ((tmp->y+tmp->height-1)*img->width +tmp->x+tmp->width)*img->nChannels , k<(int)((double)tmp->width / scale)*(int)((double)tmp->height / scale)*img->nChannels;
				j++,k++){
			tempdata[k] = data[j];
		}*/
		string out ="";	
		for(int l = imagename.length()-1; l> 0; l--){
			out = imagename[l] + out;
			if(imagename[l] == '.'){
				char num[3];
				itoa(i,num,10);	
				out = num + out;
			}
		}
		writeImage(temp,out,errmsg);
		tmp= tmp->next;
	}
}

bool facedetect::writeImage(IplImage* img,string imagename,string& errmsg){
	/* generate the name of output file and directories */
	int length = imagename.length(); 
	string out ="";		
	for(int i = imagename.length()-1; i> 0; i--){
		if(imagename[i] != '\\'){
			out = imagename[i] + out;
			/*if(imagename[i] == '.')
				out = 'b' + out;*/
		}else
			break;
	}
	string of = "";
	of = "detectedface/"+out;

	
	if(!cvSaveImage(of.c_str(),img)){ 
		errmsg = "Could not save" +  of;
		return false;
	}	
	return true;
}

string facedetect::generateFileName(int id){
	char buffer [10];
	itoa (id,buffer,10);
	string infile = "imagedir";
	int i = 0;
	do{
		infile = infile + buffer[i];
		i++;
		id = id/10;
	}while(id!= 0);
	infile = infile+".txt";
	return infile;
}

string* facedetect::getImageName(string fileName,int lenght){
	string* fileNames = new string[lenght];
	int i = 0;
	ifstream openfile(fileName.c_str());
	string imagename;
	if(openfile.is_open()){
		while(openfile.good() & i<lenght){
			getline(openfile,imagename);
			fileNames[i] = imagename;
			i++;
		}
		openfile.close();
	}
	return fileNames;
}

IplImage* facedetect::grayScale(IplImage* img){
	IplImage* temp  =cvCreateImage( cvSize( img->width, img->height ), img->depth,  1 );
	cvCvtColor( img, temp, CV_RGB2GRAY );
	return temp;
}

void facedetect::illuminationCorrection(IplImage* &img){
	//g(x,y) = f(x,y) - LPF(f(x,y)) + mean(LPF(f(x,y)))
	char* data = img->imageData;
	int a = img->imageSize;
	int b = img->width;
	int c = img->height;
	int d = img->nChannels;
	IplImage* lpf = grayScale(img);//cvCreateImage( cvSize( img->width ,img->height), img->depth, img->nChannels );
	char* lowPassFunc = lpf->imageData;
	
	int m = calculateMean(lowPassFunc,lpf->width,lpf->height,lpf->nChannels,0);
	if(m<0)
		m = 256+m;
	for(int i = 0; i<lpf->imageSize; i++){
	/*	int m = calculateMean(lowPassFunc,lpf->width,lpf->height,lpf->nChannels,i);
			if(m<0)
				m = 256+m;*/
		int l= lowPassFunc[i];
		if(l<0)
			l = 256+l;
		for(int j =0; j<img->nChannels;j++){
			
			int d= data[i*img->nChannels+j];
			if(d<0)
				d = 256+d;
			int r = d-(l-m)/2;//d - l + m;
			if(r>=128)
				r = r-256;
			data[i*img->nChannels+j] = (char)r;
		}
	}
}

void facedetect::histogramEqualizedImage(IplImage* &img){
	int Blue[256];// = new char[256];
	int Green[256];// = new char[256];
	int Red[256];// = new char[256];
	for(int i = 0; i<256; i++){
		Blue[i]	= 0;
		Green[i] = 0;
		Red[i]	= 0;
	}
	char* data = img->imageData;
	//pdf
	for(int i = 0; i<img->imageSize; i=i+3){
		int m=data[i];
		if(m<0)
			m = 256+m;
		Blue[m]++;

		m=data[i+1];
		if(m<0)
			m = 256+m;
		Green[m]++;

		m=data[i+2];
		if(m<0)
			m = 256+m;
		Red[m]++;
	}
	/*
	// ignore %2 at beginning
	int numOfIngnoredPixels = (int)((float)(img->width*img->height)*0.02);
	int remainingBluePixels = numOfIngnoredPixels;
	int remainingGreenPixels = numOfIngnoredPixels;
	int remainingRedPixels = numOfIngnoredPixels;
	int index = 0;
	while(!(remainingBluePixels == 0 && remainingGreenPixels == 0 && remainingRedPixels == 0) && index<256){
		if(remainingBluePixels>=Blue[index]){
			remainingBluePixels -=Blue[index];
			Blue[index] = 0;
		}else if(remainingBluePixels<Blue[index]){
			Blue[index] -= remainingBluePixels;
			remainingBluePixels = 0;
		}

		if(remainingGreenPixels>=Green[index]){
			remainingGreenPixels -=Green[index];
			Green[index] = 0;
		}else if(remainingGreenPixels<Green[index]){
			Green[index] -= remainingGreenPixels;
			remainingGreenPixels = 0;
		}

		if(remainingRedPixels>=Red[index]){
			remainingRedPixels -=Red[index];
			Red[index] = 0;
		}else if(remainingRedPixels<Red[index]){
			Red[index] -= remainingRedPixels;
			remainingRedPixels = 0;
		}
		index++;
	}
	// ignore %2 at end
	remainingBluePixels = numOfIngnoredPixels;
	remainingGreenPixels = numOfIngnoredPixels;
	remainingRedPixels = numOfIngnoredPixels;
	index = 255;
	while(!(remainingBluePixels == 0 && remainingGreenPixels == 0 && remainingRedPixels == 0) && index>=0){
		if(remainingBluePixels>=Blue[index]){
			remainingBluePixels -=Blue[index];
			Blue[index] = 0;
		}else if(remainingBluePixels<Blue[index]){
			Blue[index] -= remainingBluePixels;
			remainingBluePixels = 0;
		}

		if(remainingGreenPixels>=Green[index]){
			remainingGreenPixels -=Green[index];
			Green[index] = 0;
		}else if(remainingGreenPixels<Green[index]){
			Green[index] -= remainingGreenPixels;
			remainingGreenPixels = 0;
		}

		if(remainingRedPixels>=Red[index]){
			remainingRedPixels -=Red[index];
			Red[index] = 0;
		}else if(remainingRedPixels<Red[index]){
			Red[index] -= remainingRedPixels;
			remainingRedPixels = 0;
		}
		index--;
	}
	*/
	//cdf
	for(int i = 1; i<256; i++){
		Blue[i] += Blue[i-1];
		Green[i] += Green[i-1];
		Red[i] += Red[i-1];
	}
	//
	for(int i = 0; i<img->imageSize; i=i+3){
		int m=data[i];
		if(m<0)
			m = 256+m;
		int r = (int)(( (float)Blue[m]/(float)(img->imageSize/3) ) * 255);
		if(r>=128)
			r = r-256;
		data[i] = r;

		m=data[i+1];
		if(m<0)
			m = 256+m;
		r =  (int)(( (float)Green[m]/(float)(img->imageSize/3) ) * 255);
		if(r>=128)
			r = r-256;
		data[i+1] = r;

		m=data[i+2];
		if(m<0)
			m = 256+m;
		r = (int)(( (float)Red[m]/(float)(img->imageSize/3) ) * 255);
		if(r>=128)
			r = r-256;
		data[i+2] = r;
	}
}

char facedetect::calculateMean(char* func,int width,int height, int nChannels,int index){
	int mean =0;// func[0];
	int count = width*height*nChannels;
	for(int i = 0; i<count; i++){
		int val = func[i];
		if(val<0)
			val = 256+val;
		mean+=val;
	}
	mean= (mean/count);
	if(mean >= 128)
		return mean-256;
	return mean;


/*	int mean = 0;
	int count = 0;
	for(int i = -2; i<=2; i++){
		for(int j = -2; j<=2; j++){
			if( (index + i*width*nChannels + j*nChannels >=0) && (index + i*width*nChannels + j*nChannels <height*width*nChannels) ){
				int val =func[index + i*width*nChannels + j*nChannels];
				if(val<0)
					val = 256+val;
				mean += val;
				count++;
			}
		}
	}
	if(count>0){
		mean= (mean/count);
		if(mean >= 128)
			return mean-256;
		return mean;
	}
	return 0;
	*/
}
