#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "gdalwarper.h"
#include <stdio.h>
//#include"math.h"
#include<iostream>
#include"fusion.h"
//#include"cuLayer.h"
//#include"PARAMETER.h"
#define num_thread 256
#define num_block 256
__global__ void blending_pairs(float *a,float *b,float *c,float *d,float *wei,int width,int height,int w,float A,float error_lm,float error_mm,int class_num)
{
	const int tid=threadIdx.x;
	const int bid=blockIdx.x;
	const int Idx=num_thread*bid+tid;
	float r_LM,r_MM, r_center_LM,r_center_MM; 
	int row,column;
	int i,j;
	float sum1,sum2;
	float st=0.0;
	int judge;
	float dis;
	//float wei;
	float weih,result;
	int kk=0;
	int rmin,rmax,smin,smax;
	for(int kkk=Idx;kkk<width*height;kkk=kkk+num_thread*num_block)
	{
		result=0;
		judge=0;
		wei[kkk]=0;
		kk=0;
		sum1=0,sum2=0;
		row=kkk/width;
		column=kkk%width;
		//if(row==1)
		//	wei=0;
		r_center_LM =d[kkk]-b[kkk]+error_lm;
		r_center_MM=d[kkk]-c[kkk]+1.412*error_mm;
		if(column-w/2<=0)
				rmin=0;
			else
				rmin = column-w/2;

			if(column+w/2>=width-1)
				rmax = width-1;
			else
				rmax = column+w/2;

			if(row-w/2<=0)
				smin=0;
			else
				smin = row-w/2;

			if(row+w/2>=height-1)
				smax = height-1;
			else
				smax = row+w/2;
			for(i=smin;i<=smax;i++)
			{
				for(j=rmin;j<=rmax;j++)
				{
					sum1+=b[i*width+j]*b[i*width+j];
					sum2+=b[i*width+j];
				}
			}
			//if(column==30&&row==30)
			//	result=0;
			st=sqrt(sum1/(w*w)-(sum2/(w*w))*(sum2/(w*w)))/ class_num;
			for(i=smin;i<=smax;i++)
			{
				for(j=rmin;j<=rmax;j++)
				{
					if(fabs(b[kkk]-b[i*width+j])<st)
					{
						r_LM=d[i*width+j]-b[i*width+j];
						r_MM=d[i*width+j]-c[i*width+j];
						if((r_center_LM>0&&r_LM<r_center_LM)||(r_center_LM<0&&r_LM>r_center_LM))
						{
							if((r_center_MM>0&&r_MM<r_center_MM)||(r_center_MM<0&&r_MM>r_center_MM))
							{
								r_LM=fabs(r_LM)+0.0001;
								r_MM=fabs(r_MM)+0.0001;
								if(kkk==i*width+j)
									judge=1;
								dis=float((row-i)*(row-i)+(column-j)*(column-j));
								dis=sqrt(dis)/A+1.0;
								weih=1.0/(dis* r_LM*r_MM);
								wei[kkk]+=weih;
								result+=weih*(c[i*width+j]+b[i*width+j]-d[i*width+j]);
								kk++;
							}
						}
					}
				}
			}
			if(kk==0)
			{
				a[kkk]=abs(b[kkk]+c[kkk]-d[kkk])*1000;
				wei[kkk]=1000;

			}
			else
			{
				if(judge==0)
					{
					dis=1.0;
					r_LM=fabs(d[kkk]-b[kkk])+0.0001;
					r_MM=fabs(d[kkk]-c[kkk])+0.0001;
					weih=1.0/(dis* r_LM*r_MM);
					result+=weih*(b[kkk]+c[kkk]-d[kkk]);
					wei[kkk]+=weih;
				}
			a[kkk]=result;
			//if(a[kkk]<0)
			//	a[kkk]=(b[kkk]+c[kkk]-d[kkk]);
			}
	}

}
__global__ void blending(float *a,float *b,float *c,float *d,int width,int height,int w,float A,float error_lm,float error_mm,int class_num)
{
	const int tid=threadIdx.x;
	const int bid=blockIdx.x;
	const int Idx=num_thread*bid+tid;
	float r_LM,r_MM, r_center_LM,r_center_MM; 
	int row,column;
	int i,j;
	float sum1,sum2;
	float st=0.0;
	int judge;
	float dis;
	float wei;
	float weih,result;
	int kk=0;
	int rmin,rmax,smin,smax;
	for(int kkk=Idx;kkk<width*height;kkk=kkk+num_thread*num_block)
	{
		result=0;
		judge=0;
		wei=0;
		kk=0;
		sum1=0,sum2=0;
		row=kkk/width;
		column=kkk%width;
		//if(row==1)
		//	wei=0;
		r_center_LM =d[kkk]-b[kkk]+error_lm;
		r_center_MM=d[kkk]-c[kkk]+1.412*error_mm;
		if(column-w/2<=0)
				rmin=0;
			else
				rmin = column-w/2;

			if(column+w/2>=width-1)
				rmax = width-1;
			else
				rmax = column+w/2;

			if(row-w/2<=0)
				smin=0;
			else
				smin = row-w/2;

			if(row+w/2>=height-1)
				smax = height-1;
			else
				smax = row+w/2;
			for(i=smin;i<=smax;i++)
			{
				for(j=rmin;j<=rmax;j++)
				{
					sum1+=b[i*width+j]*b[i*width+j];
					sum2+=b[i*width+j];
				}
			}
			//if(column==30&&row==30)
			//	result=0;
			st=sqrt(sum1/(w*w)-(sum2/(w*w))*(sum2/(w*w)))/ class_num;
			for(i=smin;i<=smax;i++)
			{
				for(j=rmin;j<=rmax;j++)
				{
					if(fabs(b[kkk]-b[i*width+j])<st)
					{
						r_LM=d[i*width+j]-b[i*width+j];
						r_MM=d[i*width+j]-c[i*width+j];
						if((r_center_LM>0&&r_LM<r_center_LM)||(r_center_LM<0&&r_LM>r_center_LM))
						{
							if((r_center_MM>0&&r_MM<r_center_MM)||(r_center_MM<0&&r_MM>r_center_MM))
							{
								r_LM=fabs(r_LM)+0.0001;
								r_MM=fabs(r_MM)+0.0001;
								if(kkk==i*width+j)
									judge=1;
								dis=float((row-i)*(row-i)+(column-j)*(column-j));
								dis=sqrt(dis)/A+1.0;
								weih=1.0/(dis* r_LM*r_MM);
								wei+=weih;
								result+=weih*(c[i*width+j]+b[i*width+j]-d[i*width+j]);
								kk++;
							}
						}
					}
				}
			}
			if(kk==0)
			{
				a[kkk]=abs(b[kkk]+c[kkk]-d[kkk]);
				wei=10000;

			}
			else
			{
				if(judge==0)
					{
					dis=1.0;
					r_LM=fabs(d[kkk]-b[kkk])+0.0001;
					r_MM=fabs(d[kkk]-c[kkk])+0.0001;
					weih=1.0/(dis* r_LM*r_MM);
					result+=weih*(b[kkk]+c[kkk]-d[kkk]);
					wei+=weih;
				}
			a[kkk]=result/wei;
			//if(a[kkk]<0)
			//	a[kkk]=(b[kkk]+c[kkk]-d[kkk]);
			}
	}

}
void runtest1(float *BufferLandsat_0,float * BufferModis_0,float *BufferModis_1,float *out,int height,int width,int win_size ,float L_err,float M_err,int classnum,int A)
{
	float *cuda_a, *cuda_b, *cuda_c, *cuda_d;
	cudaMalloc((void**)&cuda_a, sizeof( float)*height*width);
	cudaMalloc((void**)&cuda_b, sizeof( float)*height*width);
	cudaMalloc((void**)&cuda_c, sizeof( float)*height*width);
	cudaMalloc((void**)&cuda_d, sizeof( float)*height*width);
	cudaMemcpy(cuda_b, BufferLandsat_0, sizeof(float)* height*width, cudaMemcpyHostToDevice);
	cudaMemcpy(cuda_c, BufferModis_1, sizeof(float)*height*width, cudaMemcpyHostToDevice);
	cudaMemcpy(cuda_d, BufferModis_0, sizeof( float)* height*width, cudaMemcpyHostToDevice);
	blending<<<num_block,num_thread>>>(cuda_a , cuda_b , cuda_c , cuda_d , width,height,win_size,A,L_err,M_err,classnum);
	cudaMemcpy(out,cuda_a,height*width*sizeof(float),cudaMemcpyDeviceToHost);
	//for(int i=0;i<400;i++)
	//	std::cout<<out[i]<<"  ";
	cudaFree(cuda_a);
	cudaFree(cuda_b);
	cudaFree(cuda_c);
	cudaFree(cuda_d);
}
void runtest(float *BufferLandsat_0,float * BufferModis_0,float *BufferModis_1,float *out,int height,int width,int win_size ,float L_err,float M_err,int classnum,int A)
{
	int maxnum;
	size_t ff,tt;
	//cudaSetDevice(0);
	cudaMemGetInfo(&ff, &tt);
	maxnum=(ff-sizeof(float)*win_size*win_size*num_block*num_thread*2)/(sizeof(float)*4);
	int sub_height=maxnum/width-win_size;
	//sub_height=3000;
	int kk=0;
	int i,j;
	float *sub_BufferIn11,*sub_BufferIn22,*sub_BufferIn33,*sub_out;
	for(int heiht_all=0;heiht_all<height;heiht_all+=sub_height)
	{
		int task_start=kk*sub_height;
		int task_end;
		if((kk+1)*sub_height-height<=0)
			task_end=(kk+1)*sub_height-1;
		else
			task_end=height-1; 
		int data_start,data_end;
		if(task_start-win_size/2<=0)
			data_start= 0;
		else
			data_start=task_start-win_size/2;
		if(task_end+win_size/2>=height-1)
			data_end=height-1;
		else
			data_end=task_end+win_size/2;
		int data_height=data_end-data_start+1;
		sub_BufferIn11=new float[data_height*width];
		sub_BufferIn22=new float[data_height*width];
		sub_BufferIn33=new float[data_height*width];
		sub_out=new float[data_height*width];
		int copy;
			copy=0;
			for( i=data_start;i<=data_end;i++)
			{
				for( j=0;j<width;j++)
				{
					sub_BufferIn11[copy*width+j]=BufferLandsat_0[i*width+j];
					sub_BufferIn22[copy*width+j]=BufferModis_0[i*width+j];
					sub_BufferIn33[copy*width+j]=BufferModis_1[i*width+j];
				}
				copy++;
			}
		int current=task_start-data_start;
		int task_height=task_end-task_start+1;
		runtest1(sub_BufferIn11,sub_BufferIn22,sub_BufferIn33,sub_out,data_height,width,win_size,L_err,M_err,classnum,A);
			current=task_start-data_start;
			for(int i=task_start;i<=task_end;i++)
			{
				for(int j=0;j<width;j++)
				{
					out[i*width+j]=sub_out[current*width+j];
				}
				current++;
			}
		delete []sub_BufferIn11;
		delete []sub_BufferIn22;
		delete []sub_BufferIn33;
		delete []sub_out;
		kk++;
	}
}
void runtest1_pairs(float *BufferLandsat_0,float * BufferModis_0,float *BufferModis_1,float *out,float *weight,int height,int width,int win_size ,float L_err,float M_err,int classnum,int A)
{

	float *cuda_a, *cuda_b, *cuda_c, *cuda_d,*cuda_weight;
	cudaMalloc((void**)&cuda_weight, sizeof( float)*height*width);
	cudaMalloc((void**)&cuda_a, sizeof( float)*height*width);
	cudaMalloc((void**)&cuda_b, sizeof( float)*height*width);
	cudaMalloc((void**)&cuda_c, sizeof( float)*height*width);
	cudaMalloc((void**)&cuda_d, sizeof( float)*height*width);
	cudaMemcpy(cuda_b, BufferLandsat_0, sizeof(float)* height*width, cudaMemcpyHostToDevice);
	cudaMemcpy(cuda_c, BufferModis_1, sizeof(float)*height*width, cudaMemcpyHostToDevice);
	cudaMemcpy(cuda_d, BufferModis_0, sizeof( float)* height*width, cudaMemcpyHostToDevice);
	blending_pairs<<<num_block,num_thread>>>(cuda_a , cuda_b , cuda_c , cuda_d ,cuda_weight,width,height,win_size,A,L_err,M_err,classnum);
	cudaMemcpy(out,cuda_a,height*width*sizeof(float),cudaMemcpyDeviceToHost);
	cudaMemcpy(weight,cuda_weight,height*width*sizeof(float),cudaMemcpyDeviceToHost);
	//for(int i=0;i<400;i++)
	//	std::cout<<out[i]<<"  ";
	cudaFree(cuda_a);
	cudaFree(cuda_b);
	cudaFree(cuda_c);
	cudaFree(cuda_d);
	cudaFree(cuda_weight);
}
void runtest_pairs(float *BufferLandsat_0,float * BufferModis_0,float *BufferModis_1,float *out,float *weight,int height,int width,int win_size ,float L_err,float M_err,int classnum,int A)
{
	int maxnum;
	size_t ff,tt;
	//cudaSetDevice(0);
	cudaMemGetInfo(&ff, &tt);
	maxnum=(ff-sizeof(float)*win_size*win_size*num_block*num_thread*2)/(sizeof(float)*5);
	int sub_height=maxnum/width-win_size;
   // sub_height=3000;
	int kk=0;
	int i,j;
	float *sub_BufferIn11,*sub_BufferIn22,*sub_BufferIn33,*sub_out,*sub_weight;
	for(int heiht_all=0;heiht_all<height;heiht_all+=sub_height)
	{
		int task_start=kk*sub_height;
		int task_end;
		if((kk+1)*sub_height-height<=0)
			task_end=(kk+1)*sub_height-1;
		else
			task_end=height-1; 
		int data_start,data_end;
		if(task_start-win_size/2<=0)
			data_start= 0;
		else
			data_start=task_start-win_size/2;
		if(task_end+win_size/2>=height-1)
			data_end=height-1;
		else
			data_end=task_end+win_size/2;
		int data_height=data_end-data_start+1;
		sub_BufferIn11=new float[data_height*width];
		sub_BufferIn22=new float[data_height*width];
		sub_BufferIn33=new float[data_height*width];
		sub_out=new float[data_height*width];
		sub_weight=new float[data_height*width];
		int copy;
			copy=0;
			for( i=data_start;i<=data_end;i++)
			{
				for( j=0;j<width;j++)
				{
					sub_BufferIn11[copy*width+j]=BufferLandsat_0[i*width+j];
					sub_BufferIn22[copy*width+j]=BufferModis_0[i*width+j];
					sub_BufferIn33[copy*width+j]=BufferModis_1[i*width+j];
				}
				copy++;
			}
		int current=task_start-data_start;
		int task_height=task_end-task_start+1;
		runtest1_pairs(sub_BufferIn11,sub_BufferIn22,sub_BufferIn33,sub_out,sub_weight,data_height,width,win_size,L_err,M_err,classnum,A);
			current=task_start-data_start;
			for(int i=task_start;i<=task_end;i++)
			{
				for(int j=0;j<width;j++)
				{
					out[i*width+j]=sub_out[current*width+j];
					weight[i*width+j]=sub_weight[current*width+j];
				}
				current++;
			}
		delete []sub_BufferIn11;
		delete []sub_BufferIn22;
		delete []sub_BufferIn33;
		delete []sub_out;
		delete []sub_weight;
		kk++;
	}
}
void Re_fusion(const char * BufferIn0,const char * BufferIn1,const char * BufferIn2,const char * BufferOut,int win_size,int classnum,float L_err,float M_err,int A,int pf,int pc)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO"); 
	GDALDataset *Landsat0 = (GDALDataset*) GDALOpen(BufferIn0,GA_ReadOnly);
	int width,height;
	width = Landsat0->GetRasterXSize();
	height = Landsat0->GetRasterYSize();
	//width=2000;
	//height=2000;
	float *BufferLandsat_0=new float[width*height];
	float *BufferModis_0=new float[width*height];
	float *BufferModis_1=new float[width*height];
	float *out=new float[width*height];
	GDALRasterBand* hInBand1 = Landsat0->GetRasterBand(pf);
	hInBand1->RasterIO(GF_Read,0,0,width,height,BufferLandsat_0,width,height,GDT_Float32,0,0);
	GDALDataset *MODIS0 = (GDALDataset*) GDALOpen(BufferIn1,GA_ReadOnly);
	 hInBand1 = MODIS0->GetRasterBand(pc);
	hInBand1->RasterIO(GF_Read,0,0,width,height,BufferModis_0,width,height,GDT_Float32,0,0);
	GDALDataset *Modis_1 = (GDALDataset*) GDALOpen(BufferIn2,GA_ReadOnly);
	 hInBand1 = Modis_1->GetRasterBand(pc);
	hInBand1->RasterIO(GF_Read,0,0,width,height,BufferModis_1,width,height,GDT_Float32,0,0);
	 GDALDataset *LandsatDs;
	char* driverName = "GTiff";
	GDALDriver *pDriver = (GDALDriver*)GDALGetDriverByName(driverName);
	LandsatDs = pDriver->Create(BufferOut,width,height,1,GDT_Float32,NULL);
	double* geos=new double[1];
	Landsat0->GetGeoTransform(geos);
	LandsatDs->SetGeoTransform(geos);
	LandsatDs->SetProjection(Landsat0->GetProjectionRef());
	runtest(BufferLandsat_0,BufferModis_0,BufferModis_1,out,height,width,win_size, L_err, M_err, classnum,A);
	GDALRasterBand* HOut = LandsatDs->GetRasterBand(1);
	HOut->RasterIO(GF_Write,0,0,width,height,out,width,height,GDT_Float32,0,0);
	GDALClose(Landsat0);
	GDALClose(MODIS0);
	GDALClose(Modis_1);
	GDALClose(LandsatDs);
	delete []BufferLandsat_0;
	delete []BufferModis_0;
	delete []BufferModis_1;
	delete []out;
}
void Re_fusion2(CuLayer *psensor,PARAMETER *par)
{
	int i,j,m,c;
	long now1 = clock();
	for(c=0;c<par->NUM_PREDICTIONS;c++)
	{
		psensor[2*(par->NUM_PAIRS+c)+1].resize(psensor[0].getWidth(),psensor[0].getHeight());
		if(par->NUM_PAIRS==1)
		{
			runtest(psensor[0].getData(),psensor[1].getData(),psensor[2*(par->NUM_PAIRS+c)].getData(),psensor[2*(par->NUM_PAIRS+c)+1].getData(),psensor[0].getHeight(),psensor[0].getWidth(),par->WIN_SIZE,par->L_ERR,par->M_ERR,par->class_num,par->A);
			//char* driverName = "GTiff";
			psensor[2*(par->NUM_PAIRS+c)+1].setGeoTransform(psensor[0].getGeoTransform());
			psensor[2*(par->NUM_PAIRS+c)+1].setProjection(psensor[0].getProjection());
			psensor[2*(par->NUM_PAIRS+c)+1].Write(psensor[2*(par->NUM_PAIRS+c)+1].outpath,par->G_Type);
		}
		else
		{
			float *weight=new float[psensor[0].getHeight()*psensor[0].getWidth()];
			float *weight_all=new float[psensor[0].getHeight()*psensor[0].getWidth()];
			float *single=new float[psensor[0].getHeight()*psensor[0].getWidth()];
			memset(single,0, sizeof(float)*psensor[0].getHeight()*psensor[0].getWidth());
			memset(weight_all,0, sizeof(float)*psensor[0].getHeight()*psensor[0].getWidth());
			for( i=0;i<par->NUM_PAIRS;i++)
			{
				runtest_pairs(psensor[i].getData(),psensor[i+par->NUM_PAIRS].getData(),psensor[2*(par->NUM_PAIRS+c)].getData(),psensor[2*(par->NUM_PAIRS+c)+1].getData(),weight,psensor[0].getHeight(),psensor[0].getWidth(),par->WIN_SIZE,par->L_ERR,par->M_ERR,par->class_num,par->A);
				for( m=0;m<psensor[0].getHeight();m++)
				{
					//	j=j;
					for( j=0;j<psensor[0].getWidth();j++)
					{
						single[m*psensor[0].getWidth()+j]+=psensor[2*(par->NUM_PAIRS+c)+1].getData()[m*psensor[0].getWidth()+j];
						weight_all[m*psensor[0].getWidth()+j]+=weight[m*psensor[0].getWidth()+j];
					}
				}

			}
			for( m=0;m<psensor[0].getHeight();m++)
			{
				for( j=0;j<psensor[0].getWidth();j++)
				{
					psensor[2*(par->NUM_PAIRS+c)+1].getData()[m*psensor[0].getWidth()+j]=single[m*psensor[0].getWidth()+j]/weight_all[m*psensor[0].getWidth()+j];
				}
			}
			psensor[2*(par->NUM_PAIRS+c)+1].setGeoTransform(psensor[0].getGeoTransform());
			psensor[2*(par->NUM_PAIRS+c)+1].setProjection(psensor[0].getProjection());
			psensor[2*(par->NUM_PAIRS+c)+1].Write(psensor[2*(par->NUM_PAIRS+c)+1].outpath,par->G_Type);
			delete []weight;
			delete []weight_all;
			delete []single;
		}
	}
	 printf("GPU运行时间为：%dms\n", int(((double)(clock() - now1)) / CLOCKS_PER_SEC * 1000));
}
