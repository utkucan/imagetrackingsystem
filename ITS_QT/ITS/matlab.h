#ifndef MATLAB_H
#define MATLAB_H

#include "stdio.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


class matlab{
public:
	matlab();
	
	ofstream myfile;
	char** fileList;
	string* fileList2;
	double** faceTable;
	double** faceDescs;
	int totalExamples;
	void initArrays();
	double distance(double* ,double*);
	void readFileNames(char* );
	double* returnFeatures(char*);
};
#endif
