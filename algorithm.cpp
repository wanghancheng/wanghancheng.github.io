#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <unistd.h>
#include <string>
#include <cstring> 
#include <io.h>
//working on linux
//use apt-get to install libgdal-dev, then use following code£¬use source code to compile, then remove ¡°gdal/¡± 
#include "gdal/gdal_priv.h"
#include "gdal/ogr_spatialref.h"
#pragma warning( disable : 4996 )

using namespace std;

//NDWI caculate
//NDWI=(Green-NIR)/(Green+NIR)=(Band3-Band8)/(Band3+Band8)
int NDWI(float threshold, const char *inputband3, const char *inputband8, const char *outFileName)
{
	//Input data is the data after cropping
	const char *band3 = inputband3;
	const char *band8 = inputband8;
	const char *outputFileName = outFileName;

	GDALDataset *inputDataset1;
	GDALDataset *inputDataset2;
	GDALDataset *outputDataset;

	GDALAllRegister();

	cout << "Reading image..." << endl;
	inputDataset1 = (GDALDataset *)GDALOpen(band3, GA_ReadOnly);
	inputDataset2 = (GDALDataset *)GDALOpen(band8, GA_ReadOnly);
	if (inputDataset1 == NULL || inputDataset2 == NULL)
	{
		cout << "File open failed!" << endl;
		return 1;
	}

	int imgSizeX = inputDataset1->GetRasterXSize();
	int imgSizeY = inputDataset1->GetRasterYSize();

	const char *imgFormat = "GTiff";
	GDALDriver *gdalDriver = GetGDALDriverManager()->GetDriverByName(imgFormat);
	if (gdalDriver == NULL)
	{
		cout << "File create failed!" << endl;
		return 1;
	}

	outputDataset = gdalDriver->Create(outputFileName, imgSizeX, imgSizeY, 1, GDT_Float32, NULL);

	//Get the geographic change information of the input data
	double goeInformation[6];
	inputDataset1->GetGeoTransform(goeInformation);
	//Read the geographic information of the input data and write to the output file
	const char * gdalProjection = inputDataset1->GetProjectionRef();

	outputDataset->SetGeoTransform(goeInformation);
	outputDataset->SetProjection(gdalProjection);

	cout << "Image Processing..." << endl;
	//Obtain the green band and near infrared band
	GDALRasterBand *raseterBandGreen = inputDataset1->GetRasterBand(1);
	GDALRasterBand *raseterBandNIR = inputDataset2->GetRasterBand(1);
	GDALRasterBand *outputRasterBand = outputDataset->GetRasterBand(1);
	//Request storage space of the size of a row
	float *bufferBlockGreen = (float *)CPLMalloc(sizeof(float)* imgSizeX);
	float *bufferBlockNIR = (float *)CPLMalloc(sizeof(float)* imgSizeX);
	float *outputBufferBlock = (float *)CPLMalloc(sizeof(float)* imgSizeX);

	//Calculate NDWI
	for (int i = 0; i < imgSizeY; i++)
	{
		/*CPLErr GDALRasterBand::RasterIO(GDALRWFlag eRWFlag,
		int nXOff, int nYOff, int nXSize, int nYSize,
		void * pData, int nBufXSize, int nBufYSize,
		GDALDataType eBufType,
		int nPixelSpace,
		int nLineSpace)*/

		raseterBandGreen->RasterIO(GF_Read, 0, i, imgSizeX, 1, bufferBlockGreen, imgSizeX, 1, GDT_Float32, 0, 0);
		raseterBandNIR->RasterIO(GF_Read, 0, i, imgSizeX, 1, bufferBlockNIR, imgSizeX, 1, GDT_Float32, 0, 0);
		for (int j = 0; j < imgSizeX; j++)
		{
			outputBufferBlock[j] = (bufferBlockGreen[j] - bufferBlockNIR[j]) / (bufferBlockGreen[j] + bufferBlockNIR[j]);
			if (outputBufferBlock[j] <= threshold)
			{
				outputBufferBlock[j] = 1;
			}
			else
			{
				outputBufferBlock[j] = 0;
			}
		}
		outputRasterBand->RasterIO(GF_Write, 0, i, imgSizeX, 1, outputBufferBlock, imgSizeX, 1, GDT_Float32, 0, 0);
	}
	//Release resources
	CPLFree(bufferBlockGreen);
	CPLFree(bufferBlockNIR);
	CPLFree(outputBufferBlock);
	GDALClose(inputDataset1);
	GDALClose(inputDataset2);
	GDALClose(outputDataset);
	return 0;
}

//String replaces all specified strings
string&   replace_all(string&   str, const   string&   old_value, const   string&   new_value)
{
	while (true)   {
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

//Iterate through all the files in the directory
char* listFiles(const char * dir,int i)
{
	intptr_t handle;
	_finddata_t findData;
	char *b = ".zip";
	char *b = ".SAFE";
	char *c = "B03.jp2";
	char *d = "B08.jp2";

	handle = _findfirst(dir, &findData);    // Find the first file in the directory
	if (handle == -1)
	{
		cout << "Failed to find first file!\n";
		return;
	}

	do
	{
		if (findData.attrib & _A_SUBDIR
			&& strcmp(findData.name, ".") == 0
			&& strcmp(findData.name, "..") == 0
			)    // Is a subdirectory and is not "." or ".."
		{
			if (strstr(findData.name, b) != NULL&&i==1)
			{
				return findData.name;
			}
		}
		else
		{
			if (strstr(findData.name, a) != NULL&&i == 0)
			{
				return findData.name;
			}
			else if (strstr(findData.name, c) != NULL&&i==2)
			{
				return findData.name;
			}
			else if (strstr(findData.name, d) != NULL&&i==3)
			{
				return findData.name;
			}
		}
	} while (_findnext(handle, &findData) == 0);    // Find the next file in the directory

	cout << "Done!\n";
	_findclose(handle);    // Close the search handle
	return "";
}

int main(int argc[],char* argv[]) {
	

			float threshold = 0.05;
			NDWI(threshold,b03tif,b08tif,"/home/wanghancheng/image/NDWI.tif");

	
}
