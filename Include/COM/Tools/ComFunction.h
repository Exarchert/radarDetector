#include <COM\Export.h>

namespace COM
{
	/*
	功能描述：	文件、文件夹拷贝（支持文件夹嵌套拷贝）
	参数：		strSrc	(in)	源文件路径
				strDest	(in)	目标路径
	返回值:		拷贝成功返回ture
	Author:		LiuGh
	Data:		2010.3.15
	*/
	COM_EXPORT bool FileCopy(CString strSrc,CString strDest);

	/*
	功能描述：	创建多级目录
	参数：		dirPath	(in)	目录路径
	返回值:		成功返回ture
	Author:		LiuGh
	Data:		2010.4.26
	*/

	COM_EXPORT bool CreateMultipleDirectory(const CString& dirPath);

}