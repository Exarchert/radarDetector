/*************************************************************************
File:			Cmd.h
Author:			��ƽ    
Date:			2009-11-18
Description:    ������
*************************************************************************/

#pragma once
#include <COM\Export.h>
#include <Osg\Referenced>
#include <osg\ref_ptr>
#include <vector>

namespace COM{

class CCmdObj : public osg::Referenced
{
public:
	CCmdObj(void){}
	~CCmdObj(void){}
	virtual void DoAction(){}
};


//����������
class COM_EXPORT CCmdObjQueue
{
public:
	CCmdObjQueue(void);
	~CCmdObjQueue(void);

	void EnterQueue(CCmdObj* pCmdObj);
	void GetCmdObjQueue(std::vector<osg::ref_ptr<CCmdObj>> &CmdObjQueue);
	CCmdObj *OutQueue();
	void ClearQueue();
protected:
	//��Ϣ����
	std::vector<osg::ref_ptr<CCmdObj>>	m_CmdObjQueue;
	CRITICAL_SECTION	m_CS;		//�ٽ���	
};


//��������
class COM_EXPORT CCmdCenter
{
public:
	CCmdCenter(void);
	~CCmdCenter(void);

	void Update();

	CCmdObjQueue *GetCmdObjList()
	{
		return &m_CmdObjList;
	}

protected:
	CCmdObjQueue m_CmdObjList;
};

}