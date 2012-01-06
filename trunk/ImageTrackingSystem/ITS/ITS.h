// ITS.h
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "include\opencv\cv.h"
#include "include\opencv\highgui.h"
using namespace System;
using namespace std;
namespace ITS {

	typedef struct List{
			int x,y,width,height;
			List* next;
		};

	public ref class facedetect
	{
		// TODO: Add your methods for this class here.
	private: 
		
		IplImage* image;
		int numOfFace;
		List* head;
		CvHaarClassifierCascade *cascade;
		CvMemStorage            *storage;
		int scale;
		bool isfinnised;
	public :
		facedetect(int counter)
		{
			isfinnised = false;
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
			ifstream openfile(infile.c_str());
			string line;
			if(openfile.is_open()){
				while(openfile.good()){
					getline(openfile,line);
					detect(line);
				}
				openfile.close();
			}
			isfinnised = true;
			//cvShowImage( "video", image );
		}
		bool isfin(){
			return isfinnised;
		}
		IplImage* scaleImage(IplImage* img){
			int w = img->width/ scale;
			int h = img->height/ scale;
			while( w > 400 || h > 400){
				scale++;
				w = img->width / scale;
				h = img->height/ scale;
			}
			IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), img->depth, img->nChannels );
			cvResize(img, temp);
		//	cvShowImage( "video", temp );
			return temp;
		}

		void detect(string imagename){
			//const char* imagename = "C:/Users/sercan/Desktop/seniorProj/dene6/ImageTrackingSystem/ImageTrackingSystem/m4.jpg";
			IplImage* img = NULL;
			try{
				img = cvLoadImage(imagename.c_str());//((std::string)imagename).c_str()); 
				if(!img){
					fprintf(stderr, "Can not load image %s\n", imagename);
					return;
				}
			}catch(exception e){
				return;
			}
			img->origin = 0;
			scale = 1;
			image = scaleImage(img);
			numOfFace = 0;
			head = NULL;
			CvRect* faces;
			int count = detectedFaces(faces);

			for(int i = 0; i<count; i++){
				cvRectangle( img,
							cvPoint( faces[i].x, faces[i].y ),
							cvPoint( faces[i].x + faces[i].width, faces[i].y + faces[i].height ),
							CV_RGB( 255, 0, 0 ), 3, 8, 0 );
			}
			int length = imagename.length();
			string out ="";
			
			for(int i = imagename.length()-1; i> 0; i--){
				if(imagename[i] != '\\')
					out = imagename[i] + out;
				else
					break;
			}
			string outFileName = "detectedface/"+out;
			if(!cvSaveImage(outFileName.c_str(),img)) printf("Could not save: %s\n",outFileName);
		}

		int detectedFaces(CvRect* &faceList){
			image->origin = 0;
			numOfFace = 0;
			detectFaces( image );
		//	detect_and_draw( oldC0 );
			faceList = new CvRect[numOfFace];

			struct List* tmp =head;
			for(int i = 0; i<numOfFace && tmp !=NULL; i++){
				CvRect* t = new CvRect();
				t->x = tmp->x * scale;
				t->y = tmp->y * scale;
				t->height = tmp->height* scale;
				t->width = tmp->width* scale;
				faceList[i] = *t;
				tmp= tmp->next;
			}

			clearList();
			return numOfFace;
		}

	private:

		void addList(struct List* node){
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

		void clearList(){
			struct List* tmp = head;
			while(tmp!=NULL){
				struct List* t = tmp;
				tmp = tmp->next;
				delete t;
			}
		}

		void detectFaces( IplImage *img )
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
/*
		void detect_and_draw( IplImage* img )
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
*/
	};
}
