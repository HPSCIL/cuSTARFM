#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "gdalwarper.h"
#include <stdio.h>
//#include"math.h"
#include<iostream>
#include"fusion.h"
int main(int argc, char *argv[])
{
     const char* modFile1="D:\\data\\newdata\\MODO9A1.A2009249.dat";
	const char* tifFile1="D:\\data\\newdata\\2009-249-flaash.dat";
	const char* modFile0="D:\\data\\newdata\\MODO9A1.A2009329-0.dat";
	const char* out="D:\\data\\newdata\\2009-329new-flaash-test2.tif";
	//const char* modFile1=argv[1];
	//const char* tifFile1=argv[2];
	//const char* modFile0=argv[3];
	//const char* out=argv[4];
	//int w=atoi(argv[5]);
	//int classnum=atoi(argv[6]);
 //   float L_err=atof(argv[7]);
	//float M_err=atof(argv[8]);
	//int A=atoi(argv[9]);
	long now1 = clock();
	int pf=3;
	int pc=1;
	Re_fusion(tifFile1, modFile1,modFile0,out,31,4,20,50,25,pf,pc);
	 printf("GPU运行时间为：%dms\n", int(((double)(clock() - now1)) / CLOCKS_PER_SEC * 1000));
	 cudaDeviceReset();
}