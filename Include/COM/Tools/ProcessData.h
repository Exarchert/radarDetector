#include <COM\Export.h>
#include <vector>
namespace COM
{
	/*
	功能描述：	拆分字符串
	参数：		str	(in)	源字符串
				split(in)	分隔符
	返回值:		字符串指针
	Author:		xubaolong
	Data:		2013.1.16
	*/
	COM_EXPORT std::vector<CString> SplitString(CString str,char split,int& iSubStrs);
	

	/*
	功能描述：	拆分字符串
	参数：		strIds	(in)	源字符串
				split(in)	分隔符
				list(out)   返回LONG64/float结果
	返回值:		空
	Author:		xubaolong
	Data:		2013.1.16
	*/
	COM_EXPORT void SplitString(CString strIds,char split,std::vector<LONG64>& list);
	COM_EXPORT void SplitString(CString strIds,char split,std::vector<float>& list);
	/*
	功能描述：	CString转char*
	参数：		s	(in)	源字符串
				
	返回值:		char*转换结果
	Author:		xubaolong
	Data:		2013.1.17
	*/
	COM_EXPORT char* ConvertToChar(const CString &s);


	//将TCHAR转为char    
	//*tchar是TCHAR类型指针，*_char是char类型指针    
	COM_EXPORT void TcharToChar(const TCHAR * tchar, char * _char);  
	COM_EXPORT void CharToTchar(const char * _char, TCHAR * tchar);

	COM_EXPORT std::vector<std::string> split(std::string str,std::string pattern);
	


}