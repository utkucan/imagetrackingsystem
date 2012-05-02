#include "matlab.h"

extern "C"{
	double* main2(char* filePath, int mode);// 1 desc else points
	void terminateMX();
	void initMX();
}


void matlab::readFileNames(char* file){

	string dale;
	ifstream myReadFile(file);
	char output[500];
	int i = -1;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {			
			if(i==-1){
				myReadFile >> output;
				totalExamples = atoi(output);
				fileList = new char*[totalExamples];
				fileList2 = new string[totalExamples];
				i++;
			}else{
				getline(myReadFile,dale);
				//myReadFile >> dale;
				if(dale != ""){
						fileList[i] = new char[dale.size()];
						fileList2[i]=dale; 
					for(size_t j = 0; j < dale.size(); j++)
					{
						fileList[i][j] = (char)dale[j];
					}
					//char* c = fileList2[i].c_str();
					i++;
				}
				//fileList[i++]=output;

			}
		}
	}
	myReadFile.close();

}

void matlab::initArrays(){

	readFileNames("C:\\Users\\utkucan\\Desktop\\set35\\filenames35.txt");

	faceTable = new double*[totalExamples];

	for(int i = 0 ; i<totalExamples; i++){
		faceTable[i] = new double[totalExamples];
	}

	faceDescs = new double*[totalExamples];
	for(int i = 0 ; i<totalExamples; i++){
		faceTable[i] = new double[1937];
	}
}


double matlab::distance(double* a1,double* a2){
	double sum = 0;
	for(int i = 0 ; i < 13 ; i++){
		double sSum = 0;
		for(int j = 0 ; j < 149 ; j++)
		{
			
			sSum += (a1[149*i+j]-a2[149*i+j])*(a1[149*i+j]-a2[149*i+j]);
		}
		if(i== 8 || i== 7 || i==12)
		;
		else
		sum += sqrt(sSum);

		//if(0<=i && i<6)			sum += sqrt(2*sSum);
		
	}
	return sum/13;
}
 double* matlab::returnFeatures(char* str){
	//	initMX();	
	 return main2(str,1);

 }

	matlab::matlab(){
	initMX();
	/*initArrays();

	for(int i = 0; i < totalExamples; i++){
		//printf("%s\n",fileList2[i]);
		
		std::string str = fileList2[i];
		char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
		printf("%s\n",cstr);
		faceDescs[i] = main2(cstr,1);
		// do stuff
		delete [] cstr;
	}

	myfile.open("C:\\Users\\utkucan\\Desktop\\result.txt");

	for(int k = 1; k < totalExamples; k++){
		myfile<<" "<<k;
	}
	myfile<<"\n";

	for(int i = 0; i < totalExamples; i++){
		myfile<<i+1<<" ";
		for(int k = 0; k < totalExamples; k++){
			faceTable[i][k] = distance(faceDescs[i],faceDescs[k]);
			if(faceTable[i][k]==0)
				myfile<<faceTable[i][k]<<".0000 ";
			else
				myfile<<faceTable[i][k]<<" ";
		}
		myfile<<"\n";
	}

	myfile.close();
	*/
}