#ifndef FACEDETECT
#define FACEDETECT

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "cv.h"
#include "highgui.h"
using namespace std;


class facedetect
{

	public:
		typedef struct List{
		int type;
		int x,y,width,height;
		List* next;
		IplImage* face;
	};
	

		facedetect(string fileName);
		struct List* getFaceList();
		double getScale();
		void clearList();
		
	private:
		string generateFileName(int id);
		string* getImageName(string fileName,int lenght);
		void startDetection(IplImage* &img);
		bool readImage(string imagename,IplImage* &img, string &errmsg);
		IplImage* modifyImage(IplImage* img, double& scale);
		double determineOptimalScaleFactor(IplImage* img);
		IplImage* scaleImage(IplImage* img,double scale);
		IplImage* grayScale(IplImage* img);
		void illuminationCorrection(IplImage* &img);
		void histogramEqualizedImage(IplImage* &img);
		char calculateMean(char* func,int width,int height, int nChannels,int index);
		void detectFaces();
		char* getHaarFileName(int id);
		void operate( int cascadeFileNameId);
		void addList(struct List* node);
//		void clearList();
		void extractFaceImage(IplImage* &img,double scale);
		void markFacesOnOriginalImage(IplImage* &img,double scale);
		bool writeImage(IplImage* img,string outFile,string& errmsg);
		void writeFacesImage(IplImage* img,string outFile,string& errmsg,double scale);

		IplImage* image;
		int numOfFace;
		List* head;
		double scl;
		enum {haarcascade_profileface,
			haarcascade_frontalface_alt,
			haarcascade_frontalface_alt2,
			haarcascade_frontalface_default,
			haarcascade_frontalface_alt_tree,
			haarcascade_eye,
			haarcascade_eye_tree_eyeglasses,
			haarcascade_fullbody,
			haarcascade_lefteye_2splits,
			haarcascade_lowerbody,
			haarcascade_mcs_eyepair_big,
			haarcascade_mcs_eyepair_small,
			haarcascade_mcs_lefteye,
			haarcascade_mcs_mouth,
			haarcascade_mcs_nose,
			haarcascade_mcs_righteye,
			haarcascade_mcs_upperbody,
			haarcascade_righteye_2splits,
			haarcascade_upperbody
		};
};
#endif

