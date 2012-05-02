#include "stdio.h"
//#include "main.cpp"
#include "mylib.h" /* MATLAB-generated header file */
int t;
int j;
double *ptsOut;
double *descOut;


/* Create the array */
void initMX(){
	if( !mclInitializeApplication(NULL,0) )
{
fprintf(stderr, "Could not initialize the application.\n");
exit(1);
}

if (!mylibInitialize())
{
fprintf(stderr,"Could not initialize the library.\n");
exit(1);
}

}
void terminateMX(){
	mylibTerminate();
	mclTerminateApplication();
}

double* main2(char* filePath, int mode)
{

mxArray *path_ptr, **points, **descs;



/* Call the MCR and library initialization functions */

/* Create an mxArray to input into mlfFoo */

path_ptr = mxCreateString(filePath);
//path_ptr = mxCreateString("C:\\Users\\utkucan\\Desktop\\matutku\\UtkuCan2.jpg");
//points = mxCreateNumericMatrix(2,9,mxDOUBLE_CLASS,mxREAL);
points = NULL;//mxCreateDoubleMatrix(1,18,mxREAL);
descs = NULL;


/* Call the implementation function */
/* Note the second input argument should be &y_ptr instead of y_ptr. */
//mlfFoo(1,&y_ptr,x_ptr);

mlfFacedetector(2,&points,&descs,path_ptr);

/*
printf(" - %d - num of elements\n", mxGetNumberOfElements(points));
printf(" - %d - getN\n", mxGetN(points));
printf(" - %d - getM\n", mxGetM(points));
*/
t = 0;
ptsOut = mxGetPr(points);
descOut = mxGetPr(descs);

/*for(t = 0; t<mxGetNumberOfElements(points)-1; t++){
	printf("%f %f\n",ptsOut[t],ptsOut[t+1]);
}
t = 0;
printf("descriptors");
for(t = 0; t<mxGetNumberOfElements(descs)-1; t++){
	printf("%f \n",descOut[t+1]);
}*/


/* The return value from mlfFoo is an mxArray. Use mxGetpr to get a pointer to data it contains. */
//y = mxGetPr(y_ptr);
//ret = *y;
/*if(mxIsEmpty(points)){
	printf("bos\n");
}else{
	printf("dolu\n");
}*/
	
/* Display the output of the call to mlfFoo. */
//printf("Ubercharge is ready!\n");
//getchar();
/* Call the library termination function */

mxDestroyArray(path_ptr);
mxDestroyArray(points);
//getchar();
if(mode == 1)
	return descOut;

return ptsOut;
}