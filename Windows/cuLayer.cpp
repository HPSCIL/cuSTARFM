#include"cuLayer.h"
CuLayer::CuLayer()
	{
		this->m_data = NULL;
		this->m_width = 0;
		this->m_height = 0;
		//this->m_noData = this->getDefaultNoDataVal();
		this->m_projection = "";
		memset(m_adfGeoTransform, 0, sizeof(double) * 6);
	}

	CuLayer::CuLayer(float* data, int width, int height)
	{
		if (data == NULL)
		{
			printf("data is Null.");
			exit(EXIT_FAILURE);
		}
		if (0 == width)
		{
			printf("width==0.");
			exit(EXIT_FAILURE);
		}
		if (0 == height)
		{
			printf("height==0.");
			exit(EXIT_FAILURE);
		}
		this->m_width = width;
		this->m_height = height;

		this->m_data = new float[this->m_width*this->m_height];
		memcpy(this->m_data, data, sizeof(float)*this->m_width*this->m_height);

		//this->m_noData = this->getDefaultNoDataVal();
		this->m_projection = "";
		memset(m_adfGeoTransform, 0, sizeof(double) * 6);

	}
	void CuLayer::resize(int width, int height)
	{
		if (0 == width)
		{
			printf("width==0.");
			exit(EXIT_FAILURE);
		}
		if (0 == height)
		{
			printf("height==0.");
			exit(EXIT_FAILURE);
		}

		if (0 == m_width || 0 == m_height)
		{
			m_data = NULL;
		}


		if (m_width != width||m_height != height)
		{
			if (m_data!=NULL)
				delete[] m_data;
			this->m_data = new float[width*height];
			m_width = width;
			m_height = height;
		}
	}
	CuLayer::CuLayer(int width, int height)
	{
		if (0 == width)
		{
			printf("width==0.");
			exit(EXIT_FAILURE);
		}
		if (0 == height)
		{
			printf("height==0.");
			exit(EXIT_FAILURE);
		}
		this->m_width = width;
		this->m_height = height;
		this->m_data = new float[width*height];

	//	this->m_noData = this->getDefaultNoDataVal();
		this->m_projection = "";
		memset(m_adfGeoTransform, 0, sizeof(double) * 6);
	}


	

	CuLayer::~CuLayer()
	{
		if (m_data != NULL)
		{
			delete[] m_data;
			m_data = NULL;
		}
	}

void CuLayer::setGeoTransform(double* adfGeoTransform)
	{
		if (adfGeoTransform == NULL)
		{
			//print("adfGeoTransform is null.");
			exit(EXIT_FAILURE);
		}

		memcpy(m_adfGeoTransform, adfGeoTransform, sizeof(double) * 6);

	}

	CuLayer::CuLayer(const CuLayer &culayer)
	{

		if (culayer.m_data == NULL)
		{
			printf("data is Null.");
			exit(EXIT_FAILURE);
		}
		if (0 == culayer.m_width)
		{
			printf("width==0.");
			exit(EXIT_FAILURE);
		}
		if (0 == culayer.m_height)
		{
			printf("height==0.");
			exit(EXIT_FAILURE);
		}
		this->m_width = culayer.m_width;
		this->m_height = culayer.m_height;
		this->m_noData = culayer.m_noData;
		this->m_projection = culayer.m_projection;
		memcpy(this->m_adfGeoTransform, culayer.m_adfGeoTransform, sizeof(double) * 6);

		this->m_data = new float[m_width*m_height];

		memcpy(this->m_data, culayer.m_data, sizeof(float)*m_width*m_height);
	}
	int CuLayer::Read(string filePath,int pf)
	{

		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
		GDALDataset *ReadDataSet = (GDALDataset*)GDALOpen(filePath.c_str(), GA_ReadOnly);
		if (ReadDataSet == NULL)
		{
			return -1;
		}

		this->m_width = ReadDataSet->GetRasterXSize();
		this->m_height= ReadDataSet->GetRasterYSize();
		int bandCount = ReadDataSet->GetRasterCount();
		if (bandCount <pf||pf<0)
		{
			return -1;
		}
		
		ReadDataSet->GetGeoTransform(this->m_adfGeoTransform);

		m_noData = float(ReadDataSet->GetRasterBand(1)->GetNoDataValue());
		m_projection = ReadDataSet->GetProjectionRef();

		float *pImageBuf = NULL;
		pImageBuf = new float[m_width*m_height];

		if (NULL == pImageBuf)
		{
			delete ReadDataSet; ReadDataSet = NULL;
			return -2;
		}

		//GDALfloat GDT_Float32 = getGDALfloat(typeid(float).name());

		GDALRasterBand* hInBand1 = ReadDataSet->GetRasterBand(pf);
		if ( hInBand1->RasterIO(GF_Read, 0, 0, this->m_width, this->m_height, pImageBuf, this->m_width, this->m_height, GDT_Float32, 0, 0) == CE_Failure)
		{
			delete ReadDataSet; ReadDataSet = NULL;
			delete[] pImageBuf; pImageBuf = NULL;
			return -3;
		}
		
		this->m_data = pImageBuf;

		delete ReadDataSet;

		return 0;

	}
	
	char* findImageTypeGDAL(std::string DstImgFileName)
	{
		int index = DstImgFileName.find_last_of('.');
		std::string dstExtension = DstImgFileName.substr(index + 1, DstImgFileName.length() - index - 1);
		char *Gtype = NULL;
		if (dstExtension == "bmp") Gtype = "BMP";
		else if (dstExtension == "jpg") Gtype = "JPEG";
		else if (dstExtension == "png") Gtype = "PNG";
		else if (dstExtension == "tif") Gtype = "GTiff";
		else if (dstExtension == "gif") Gtype = "GIF";
		else Gtype = NULL;

		return Gtype;
	}
	int CuLayer::Write(string filePath,string Type)
	{
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
		char *GType = NULL;
		GType = findImageTypeGDAL(filePath);
		if (GType == NULL)
		{
			GType ==Type.c_str();
		}
		if (GType == NULL)
		{
			return -4;
		}

		GDALDriver *pMemDriver = NULL;
		pMemDriver = GetGDALDriverManager()->GetDriverByName(GType);
		if (pMemDriver == NULL)
		{
			return -5;
		}

		int bandCount = 1;

		//GDALDataType GDT_Float32 = getGDALDataType(typeid(float).name());
		
		GDALDataset * pMemDataSet = pMemDriver->Create(filePath.c_str(), this->m_width, this->m_height, bandCount, GDT_Float32, NULL);

		if (pMemDataSet == NULL)
		{
			printf("Create filePath failed.");
			return -6;
		}

		pMemDataSet->SetGeoTransform(this->m_adfGeoTransform);
		pMemDataSet->SetProjection(this->m_projection.c_str());


		GDALRasterBand *pBand = NULL;
		int nLineCount = this->m_width * bandCount;
		int silceSize = this->m_width*this->m_height;
		
		for (int i = 1; i <= bandCount; i++)
		{
			pBand = pMemDataSet->GetRasterBand(i);
			pBand->RasterIO(GF_Write,
				0,
				0,
				this->m_width,
				this->m_height,
				this->m_data,
				this->m_width,
				this->m_height,
				GDT_Float32,
				0,
				0);
			//pBand->SetNoDataValue(this->m_noData);
		}

		GDALClose(pMemDataSet);
		GetGDALDriverManager()->DeregisterDriver(pMemDriver);
		return 0;
	}
