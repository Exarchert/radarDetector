/*************************************************************************
File:			Event.h
Author:			张平    
Date:			2009-11-18
Description:    CEvent基类.事件类

*************************************************************************/
#pragma once
#include <COM\Export.h>
#include <Osg\Referenced>
#include <osg\ref_ptr>
#include <vector>

namespace COM{

class COM_EXPORT CMessage
{
public:
	CMessage(void){
		m_Type = 0;
		m_Massage = 0;
		m_ObjID = 0;
		m_Ptr = NULL;
		m_Para1 = 0;
		m_Para2 = 0;
	}
	~CMessage(void){}

	//消息类型
	DWORD	m_Type;
	DWORD	m_Massage;
	LONG64	m_ObjID;
	CString m_objIDStr;
	void *	m_Ptr;
	DWORD	m_Para1;
	DWORD	m_Para2;
	CString m_XMLDes;
};


//消息队列
class COM_EXPORT CMessageQueue
{
public:
	CMessageQueue(void);
	~CMessageQueue(void);

	void EnterQueue(CMessage Msg);
	void GetMsgQueue(std::vector<CMessage> &MsgQueue, bool bClear = true);
	bool OutQueue(CMessage &Msg);
	void ClearQueue();
protected:
	//消息队列
	std::vector<CMessage>	m_MsgQueue;
	CRITICAL_SECTION			m_CS;		//临界区	
};

//事件回调类
class COM_EXPORT CEventCallback : public osg::Referenced
{
public:
	CEventCallback(void);
	~CEventCallback(void);

	CString m_Name;

	//匹配方式：类型匹配
	enum{
		Match_Style_ObjType = 0x00000001,
		Match_Style_MassageType = 0x00000002,
		Match_Style_ObjID = 0x00000004,
	};
	//设置和获取匹配方式
	void SetMatchStyle(DWORD MatchStyle);
	DWORD GetMatchStyle();

	void SetMatchValue(DWORD MatchType, DWORD MatchMassage, LONG64 MatchObjID);
	void GetMatchValue(DWORD &MatchType, DWORD &MatchMassage, LONG64 &MatchObjID);

	//设置和获取匹配对象的类型
	void SetMatchType(DWORD MatchType);
	DWORD GetMatchType();

	//设置和获取匹配消息类型
	void SetMatchMassage(DWORD MatchMassage);
	DWORD GetMatchMassage();

	//设置和获取匹配对象的类型
	void SetMatchObjID(LONG64 MatchObjID);
	LONG64 GetMatchObjID();

	void Update(const CMessage &EventMessage);

	//回调函数
	virtual void DoAction(const CMessage &EventMessage);

protected:
	DWORD	m_MatchStyle;
	DWORD	m_MatchType;
	DWORD	m_MatchMassage;
	LONG64	m_MatchObjID;
};

//事件和消息中心
class COM_EXPORT CEventMsgCenter
{
public:
	CEventMsgCenter(void);
	~CEventMsgCenter(void);

	//注册回调事件
	void RegisterEventCallback(CEventCallback * EventCallback);
	void RemoveEventCallback(CString Name);

	void Update();

	CMessageQueue *GetMessageList()
	{
		return &m_EMList;
	}

protected:
	CMessageQueue m_EMList;

	std::vector<osg::ref_ptr<CEventCallback>> m_EventCallbacks;

	CRITICAL_SECTION			m_CS;		//临界区	
private:
	std::vector<CString> m_RemoveNames;


};

}