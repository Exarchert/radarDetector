#include <COM\Export.h>

namespace COM
{
	/*
	����������	�ļ����ļ��п�����֧���ļ���Ƕ�׿�����
	������		strSrc	(in)	Դ�ļ�·��
				strDest	(in)	Ŀ��·��
	����ֵ:		�����ɹ�����ture
	Author:		LiuGh
	Data:		2010.3.15
	*/
	COM_EXPORT bool FileCopy(CString strSrc,CString strDest);

	/*
	����������	�����༶Ŀ¼
	������		dirPath	(in)	Ŀ¼·��
	����ֵ:		�ɹ�����ture
	Author:		LiuGh
	Data:		2010.4.26
	*/

	COM_EXPORT bool CreateMultipleDirectory(const CString& dirPath);

}