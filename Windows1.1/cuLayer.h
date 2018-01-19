#include <string.h>
#include <gdal_priv.h>
#include<string>
#include "gdal.h"
#include "gdalwarper.h"
#include <stdio.h>
//#include"math.h"
#include<iostream>
using namespace std;
	class CuLayer
	{
	public:
		CuLayer();
		CuLayer(float* data, int width, int height);
		CuLayer(int width, int height);
		~CuLayer();
		CuLayer(const CuLayer &culayer);
		int Read(string filePath,int pf);
		int Write(string filePath,string Type);
		void resize(int width, int height);
		void resize(float* data, int width, int height);
		const int getWidth()const{ return this->m_width; };
		const int getHeight()const{ return this->m_height; };
		float* getData(){ return this->m_data; };
		float getNoDataValue(){ return this->m_noData; };
		double getCellWidth(){ return this->m_adfGeoTransform[1]; };
		double getCellHeight(){ return this->m_adfGeoTransform[5]; };
		double *getGeoTransform(){return this->m_adfGeoTransform;};
		string getProjection(){ return this->m_projection; };
		void setNoDataValue(float nodata){ this->m_noData = nodata; };
		void setCellHeight(double cellHeight){ this->m_adfGeoTransform[4] = cellHeight; };
		void setCellWidth(double cellWidth){ this->m_adfGeoTransform[1] = cellWidth; };
		void setGeoTransform(double* adfGeoTransform);
		void setProjection(string projection){ this->m_projection = projection; };
		char outpath[1000];
	private:

		//float getDefaultNoDataVal();


		void layerAdd(float* layer1data, float* layer2data, float* layerResult, int width, int height);

		template<class ParamType>
		void layerAdd(float* layer1data, float* layerResult, ParamType param, int width, int height);

		void layerSubtract(float* layer1data, float* layer2data, float* layerResult, int width, int height);
		template<class ParamType>
		void layerSubtract(float* layer1data, float* layerResult, ParamType param, int width, int height);
		void layerMultiply(float* layer1data, float* layer2data, float* layerResult, int width, int height);

		template<class ParamType>
		void layerMultiply(float* layer1data, float* layerResult, ParamType param, int width, int height);

		void layerDivide(float* layer1data, float* layer2data, float* layerResult, int width, int height);

		template<class ParamType>
		void layerDivide(float* layer1data, float* layerResult, ParamType param, int width, int height);




	private:

		float* m_data;
		int m_width;
		int m_height;

		float m_noData;
		double m_adfGeoTransform[6];
		string m_projection;
		
		
	};

