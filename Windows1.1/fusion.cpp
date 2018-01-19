#include"fusion.h"
#include <cuda_runtime.h>
int main(int argc, char *argv[])
{
	if(argc!=2) 
	{
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
	//cudaSetDevice(1);
	Re_fusion2(culayer,par);
	delete []culayer;
	//cudaDeviceReset();
	return 0;
}