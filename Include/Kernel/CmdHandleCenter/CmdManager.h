#pragma once

#include <Kernel/Export.h>
#include <vector>
#include <map>
#include <osg/ref_ptr>
#include <Kernel/CmdHandleCenter/CmdHandle.h>
#import   "msxml3.dll"

namespace Kernel
{
class KERNEL_EXPORT CCmdManager
{
public:
	CCmdManager(void);
	~CCmdManager(void);
											   
	//添加命令解析器
	void AddCmdHandle(CCmdHandle *pCmdHandle);

	//处理命令
	CString TreatCmd(CString Cmd);

	CString TreatCmd(CString cmd,CComDispatchDriver dispCall);
protected:
	std::vector<osg::ref_ptr<CCmdHandle>> m_CmdHandles;
	std::map<CString, osg::ref_ptr<CCmdHandle>> m_mapCmdHandles;
public:
	void GetMenuNames(std::map<int,CString>& menuMap);
	void GetMenuPos(POINT& pt);
	void SetMenuIndex(int index);
	void SetMenuName(CString menuName);
public:
	static std::map<int,CString>	m_MenuMap;
	static int		m_MenuIndex;
	static CString	m_MenuName;
	static POINT	m_MenuPos;
	static LONG64	m_ObjID;
	static std::map<CString,CString> m_FontMap;
};
}
