#include <COM\Export.h>
#include <vector>
namespace COM
{
	/*
	����������	����ַ���
	������		str	(in)	Դ�ַ���
				split(in)	�ָ���
	����ֵ:		�ַ���ָ��
	Author:		xubaolong
	Data:		2013.1.16
	*/
	COM_EXPORT std::vector<CString> SplitString(CString str,char split,int& iSubStrs);
	

	/*
	����������	����ַ���
	������		strIds	(in)	Դ�ַ���
				split(in)	�ָ���
				list(out)   ����LONG64/float���
	����ֵ:		��
	Author:		xubaolong
	Data:		2013.1.16
	*/
	COM_EXPORT void SplitString(CString strIds,char split,std::vector<LONG64>& list);
	COM_EXPORT void SplitString(CString strIds,char split,std::vector<float>& list);
	/*
	����������	CStringתchar*
	������		s	(in)	Դ�ַ���
				
	����ֵ:		char*ת�����
	Author:		xubaolong
	Data:		2013.1.17
	*/
	COM_EXPORT char* ConvertToChar(const CString &s);


	//��TCHARתΪchar    
	//*tchar��TCHAR����ָ�룬*_char��char����ָ��    
	COM_EXPORT void TcharToChar(const TCHAR * tchar, char * _char);  
	COM_EXPORT void CharToTchar(const char * _char, TCHAR * tchar);

	COM_EXPORT std::vector<std::string> split(std::string str,std::string pattern);
	


}