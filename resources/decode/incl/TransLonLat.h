#ifndef _TRANSLONLAT_
#define _TRANSLONLAT_
#include "SMutex.h"
#define MAP_OFFSET_FILE_NAME "offset.dat"

class CTransLonLat
{
public:
	~CTransLonLat(void);
	static CTransLonLat* instance();
	bool TransLonLat(double& dLat, double& dLng);
	bool InitOffsetLibary(const char* szOffsetFile);
private:
	CTransLonLat(void);
	void Close();
	unsigned long m_lFileSize;
	void*         m_pBuffer;
	bool          m_bInit;
	sfc::Mutex    m_Mutex;
};
#endif
