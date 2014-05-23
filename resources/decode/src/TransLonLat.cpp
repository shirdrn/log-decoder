#include "TransLonLat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define M_PI 3.14159265358979323846264338327950288
#define M_E  2.71828182845904523536028747135266250
typedef struct offset_data 
{
	short lng;//12151表示121.51
	short lat;//3130表示31.30
	short x_off;//地图x轴偏移像素值
	short y_off;//地图y轴偏移像素值
}offset_data;

static double lngToPixel(double lng, int zoom) 
{
	return (lng + 180) * (256L << zoom) / 360;
}

static double latToPixel(double lat, int zoom) 
{
	double siny =sin(lat * M_PI / 180);
	double y = log((1 + siny) / (1 - siny));
	return (128 << zoom) * (1 - y / (2 * M_PI));
}

static double pixelToLng(double pixelX, int zoom) 
{
	return pixelX * 360 / (256L << zoom) - 180;
}

static double pixelToLat(double pixelY, int zoom) 
{
	double y = 2 * M_PI * (1 - pixelY / (128 << zoom));
	double z = pow(M_E, y);
	double siny = (z - 1) / (z + 1);
	return asin(siny) * 180 / M_PI;
}

static int compare_offset_data(offset_data *data1, offset_data *data2)
{
	int det_lng = (data1->lng)-(data2->lng);
	if (det_lng!=0)
	{
		return det_lng;
	}
	else
	{
		return (data1->lat)-(data2->lat);
	}
}

CTransLonLat::CTransLonLat(void)
{
	m_lFileSize    = 0;
	m_pBuffer      = NULL;
	m_bInit        = false;
}

CTransLonLat::~CTransLonLat(void)
{
	Close();
}

CTransLonLat* CTransLonLat::instance()
{
	static CTransLonLat transLonLat;
	return &transLonLat;
}

bool CTransLonLat::InitOffsetLibary(const char* szOffsetFile)
{
	FILE *pFile = NULL;
	sfc::AutoMutex auto_mutex(&m_Mutex);
	if (m_bInit)
	{
		return true;
	}
	Close();
	char szFileName[300] = {0};
	sprintf(szFileName, "%s/%s", szOffsetFile, MAP_OFFSET_FILE_NAME);
	pFile = fopen(szFileName, "rb");
	if(NULL == pFile)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	m_lFileSize = ftell(pFile);
	if ( pFile <=0  )
	{
		fclose(pFile);
		return false;
	}
	m_pBuffer=(char *)malloc(m_lFileSize+1); 
	if ( NULL == m_pBuffer )
	{
		fclose(pFile);
		return false;
	}
	fseek(pFile,     0, SEEK_SET);
	memset(m_pBuffer,0, m_lFileSize+1);
	fread(m_pBuffer, m_lFileSize, 1, pFile);
	fclose(pFile);
	m_bInit = true;
	return true;
}

bool CTransLonLat::TransLonLat(double& dLat, double& dLng)
{
	if ((NULL == m_pBuffer)||(!m_bInit))
	{
		return false;
	}
	offset_data search_data;
	search_data.lat = (int)(dLat*100);
	search_data.lng = (int)(dLng*100);
	int st = m_lFileSize/sizeof(offset_data);
	st = sizeof(offset_data);
	offset_data *ret = (offset_data *) bsearch(&search_data, m_pBuffer, m_lFileSize/sizeof(offset_data), sizeof(offset_data), (int (*)(const void *, const void *))compare_offset_data);//折半查找
	double pixY = latToPixel(dLat, 18);
	double pixX = lngToPixel(dLng, 18);
	if (NULL == ret) 
	{
		return false;
	}
	pixY += ret->y_off;
	pixX += ret->x_off;
	dLat = pixelToLat(pixY, 18);
	dLng = pixelToLng(pixX, 18);
	return true;
}

void CTransLonLat::Close()
{
	if (NULL != m_pBuffer)
	{
		free(m_pBuffer); 
		m_pBuffer = NULL;
		m_lFileSize = 0;
		m_bInit     = false;
	}
}
