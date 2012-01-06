#include "facedetect.h"
#include "stdafx.h"

facedetect::facedetect(IplImage* img )
{
	image = img;
	numOfFace = 0;
	head = NULL;
}


facedetect::~facedetect(void)
{
}

int facedetect::detectedFaces(CvRect* &faceList){
	image->origin = 0;
	detectFaces( image );
//	detect_and_draw( oldC0 );
	faceList = new CvRect[numOfFace];

	struct List* tmp =head;
	for(int i = 0; i<numOfFace && tmp !=NULL; i++){
		CvRect* t = new CvRect();
		t->x = tmp->x;
		t->y = tmp->y;
		t->height = tmp->height;
		t->width = tmp->width;
		faceList[i] = *t;
		tmp= tmp->next;
	}

	clearList();
	return numOfFace;
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

void facedetect::clearList(){
	struct List* tmp = head;
	while(tmp!=NULL){
		struct List* t = tmp;
		tmp = tmp->next;
		delete t;
	}
}

void facedetect::detectFaces( IplImage *img )
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
		cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
		storage = cvCreateMemStorage( 0 );
		assert( cascade && storage);

		int i;
 
		/* detect faces */
		CvSeq *faces = cvHaarDetectObjects(
				img,
				cascade,
				storage,
				1.1,
				3,
				 CV_HAAR_DO_CANNY_PRUNING,
				cvSize( 40, 40 ) );
 
		/* for each face found, draw a red box */
		for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
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
	}
/*	struct List* r =head;
	while(r !=NULL){
		cvRectangle( img,
					cvPoint( r->x, r->y ),
					cvPoint( r->x + r->width, r->y + r->height ),
					CV_RGB( 255, 0, 0 ), 1, 8, 0 );
		r = r->next;
	}*/
    /* display video */
//    cvShowImage( "video", img );
}

void facedetect::detect_and_draw( IplImage* img )
{
    int scale = 1;

    // Create a new image based on the input image
    IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );

    // Create two points to represent the face locations
    CvPoint pt1, pt2;
    int i;

    // Clear the memory storage which was used before
    cvClearMemStorage( storage );

    // Find whether the cascade is loaded, to find the faces. If yes, then:
    if( cascade )
    {

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
        CvSeq* faces = cvHaarDetectObjects( img, cascade, storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );

        // Loop the number of faces found.
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
           // Create a new rectangle for drawing the face
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

            // Find the dimensions of the face,and scale it if necessary
            pt1.x = r->x*scale;
            pt2.x = (r->x+r->width)*scale;
            pt1.y = r->y*scale;
            pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
            cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
        }
    }

    // Show the image in the window named "result"
    cvShowImage( "result", img );

    // Release the temp image created.
    cvReleaseImage( &temp );
}
