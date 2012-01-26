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
	public:
		facedetect(int counter);
	private:
		bool readImage(string imagename,IplImage* &img, string &errmsg);
		IplImage* modifyImage(IplImage* img, double& scale);
		double determineOptimalScaleFactor(IplImage* img);
		IplImage* scaleImage(IplImage* img,double scale);
		void detectFaces();
		void operate( char *cascadeFileName);
		void addList(struct List* node);
		void clearList();
		void markFacesOnOriginalImage(IplImage* &img,double scale);
		bool writeImage(IplImage* img,string outFile,string& errmsg);
		
		IplImage* image;
		int numOfFace;
		List* head;

	};
}
