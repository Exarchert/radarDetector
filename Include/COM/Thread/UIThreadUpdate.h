/*********************************************************************
Copyright (C),  
�ļ���UIThreadUpdate.h UIThreadUpdate.cpp
���ߣ���ƽ
�汾��1.0
���ڣ�2009-11-23
������UI�̸߳��»���

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
���ߣ�л��ҫ            
���ڣ�2009-11-23
��������ӶԻ�������Ա�Լ���ȡ�ķ������޸ķ�����Ϣ�Ľӿ�

���ߣ�л��ҫ            
���ڣ�2009-11-24
�������޸���Ϣ���ݷ�ʽ���������ٸ���ע�ᴰ����Ϣ�ص�����������ÿ��
      �����Լ�ͨ��MFC����Ϣӳ��ػ���Ϣ��������ֻ������ָ������Ϣ
*********************************************************************/

#pragma once 

#include <Com\Export.h>
namespace COM	{

class COM_EXPORT CUIThreadUpdate
{
public:
	CUIThreadUpdate(void);
	virtual ~CUIThreadUpdate(void);

	//���͸�����Ϣ
	void SendUpdateMsg(int msg,WPARAM wParam,LPARAM lParam);

	//���º���
	virtual void Update();

	//���ô��ھ��
	void SetWndHandle(HWND hWnd);

	//��ȡ���ھ��
	HWND GetWndHandle();
private:
	HWND m_hChildWnd;
};

}