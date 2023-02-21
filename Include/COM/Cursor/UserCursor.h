/******************************************
File name:			CUserCursor.h
Author:				�����    
Date:				2009-09-17
Description:		���������
*******************************************/
#pragma once
#include <COM\Export.h>
#include <osgViewer\Viewer>

namespace COM
{
	class COM_EXPORT CUserCursor
	{
	public:
		CUserCursor(osgViewer::Viewer* viewer);
		~CUserCursor(void);

		/**********************************************************
		����������ע���µĹ����Դ����Դȫ·�����������а�
		cursorPatn			in				�����Դ·��
		cursorName			in				�Զ�������
		�ɹ�����true�����򷵻�false��
		***********************************************************/
		bool RegistCursor(const std::string cursorPath, const std::string cursorName);

		/*****************************************************
		�����������������Ĺ����Ϊ��ǰ���
		cursorName			in				�Զ�������
		�ɹ�����true�����򷵻�false��
		******************************************************/
		bool SetCursor(const std::string cursorName);
	private:
		osgViewer::GraphicsWindow* m_GW;
	};
}
