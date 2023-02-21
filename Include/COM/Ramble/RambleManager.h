/************************************************************************
Copyright (C)  
�ļ���RambleManager.h RambleManager.cpp
���ߣ��ױ�
�汾��1.0
���ڣ�2009-06-18
���������β����������࣬���ں��û�����ӿڣ���װ�����������.

�޸ļ�¼��
����					����				����
-------------------------------------------------------------------------
��־��					2009-11-13			��ֲ�ϰ汾���޸Ĳ��ֳ�Ա����.

************************************************************************/

#pragma once 

#include <COM\Export.h>
#include <COM\Ramble\RambleBase.h>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>


namespace COM	{

class COM_EXPORT CRambleManager : public osgGA::GUIEventHandler
{
public:
	CRambleManager();
	virtual ~CRambleManager();

	//�¼�����
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	//����Windows���͵���Ϣ
	virtual bool handle(UINT msg, WPARAM wParam, LPARAM lParam);

	//���úͻ�ȡ��ǰ��������
	void SetRambler(CRambleBase *rambler);
	CRambleBase *GetRambler() const;

	//���������Ƿ���
	bool EnableRamble(bool bEnable=true);

protected:
	osg::ref_ptr<CRambleBase>	m_rambler;
	bool						m_bEnable;
};

}
