#pragma once
#include <Com\Export.h>
#include <vector>
namespace COM	{
class COM_EXPORT CLoadDown
{
public:
	CLoadDown(void);
	~CLoadDown(void);

	static bool Read(CString FileNameUrl, CString CachePath, CString& LocalFile);
	static std::vector<CString> m_DelFileLists;
};

}