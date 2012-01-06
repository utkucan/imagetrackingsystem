#pragma once
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
using namespace System;
class facedetect
{
public:
	facedetect(IplImage* );
	~facedetect(void);
	int detectedFaces(CvRect* &faceList);
	typedef struct List{
		int x,y,width,height;
		List* next;
	};
private:
	IplImage* image;
	int numOfFace;
	List* head;
	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;
	
	void addList(struct List* node);
	void clearList();
	void detectFaces( IplImage *img );
	void detect_and_draw( IplImage* img );
};

