//#include"cuLayer.h"

#include"fusion.h"
int main(int argc, char *argv[])
{
  /* const char* modFile1="D:\\data\\newdata\\MODO9A1.A2009249.dat";
	const char* tifFile1="D:\\data\\newdata\\2009-249-flaash.dat";
	const char* modFile0="D:\\data\\newdata\\MODO9A1.A2009329-0.dat";
	const char* out="D:\\data\\newdata\\2009-329new-flaash-test2.tif";*/
	//if(argc!=12)
	//{
	//	std::cout<<"参数输入有误"<<std::endl;
	//	return -1;
	//}
	////const char* modFile1=argv[1];
	////const char* tifFile1=argv[2];
	////const char* modFile0=argv[3];
	////const char* out=argv[4];
	////int w=atoi(argv[5]);
	////int classnum=atoi(argv[6]);
 //   //  float L_err=atof(argv[7]);
	////float M_err=atof(argv[8]);
	////int A=atoi(argv[9]);
	////int pf=atoi(argv[10]);
	////int pc=atoi(argv[11]);
	//long now1 = clock();
	//int pf=3;
	//int pc=1;
	//int w=31;
	//int class_num=4;
	//float L_err=20;
	//float M_err=50;
	//float A=25;
 //   Re_fusion(tifFile1, modFile1,modFile0,out,w,class_num,L_err,M_err,A,pf,pc);
	// printf("GPU运行时间为：%dms\n", int(((double)(clock() - now1)) / CLOCKS_PER_SEC * 1000));
	if(argc!=2) {
		usage(argv[0]);
		exit(1);
	}
	PARAMETER *par=new PARAMETER[1];
	//par=malloc(sizeof(PARAMETER));
	CuLayer  *culayer=new  CuLayer[20];
	if(parseParameters(argv[1], culayer, par) == -1) 
	{
		printf("Retrieve input parameters error! \n");
		usage(argv[0]);
		exit(1);
	}
	Re_fusion2(culayer,par);
	delete []culayer;
	cudaDeviceReset();
}