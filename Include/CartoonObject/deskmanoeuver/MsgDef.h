/*************************************************************************
FileName��MsgDef.h
Author:	Liugh
Date��2010-01-22
Description��Manoeuver Message define
**************************************************************************/
#pragma once

namespace CartoonObject
{
	enum
	{
		//UI->��ά
		WM_MSG_BEGINE		= 	 WM_USER + 0x00500,
		WM_MSG_DISASTER		=	 WM_MSG_BEGINE + 1,		//�ֺ�
		WM_MSG_FIREEQUIP	=	 WM_MSG_BEGINE + 2,		//�����豸
		WM_MSG_MOVEMENT		=	 WM_MSG_BEGINE + 3,		//��
		WM_MSG_ROLE			=	 WM_MSG_BEGINE + 4,		//����


		//��������
		Window_Message_StartDeskTest	=	WM_MSG_BEGINE + 10,	//��������
		Window_Message_ScriptRegister	=	WM_MSG_BEGINE + 11,	//��������
		Window_Message_DeskFolder_Create=	WM_MSG_BEGINE + 12,	//���ݿ��½�


		//��ά->UI
		WM_WND_MSG_BEGINE		= 	WM_USER + 0x00600,
		WM_WND_MSG_DISASTER		=	WM_WND_MSG_BEGINE + 1,	//�ֺ�
		WM_WND_MSG_FIREEQUIP	=	WM_WND_MSG_BEGINE + 2,	//�����豸
		WM_WND_MSG_MOVEMENT		=	WM_WND_MSG_BEGINE + 3,	//��
		WM_WND_MSG_ROLE			=	WM_WND_MSG_BEGINE + 4,	//��
		WM_WND_MSG_HOTEREA		=	WM_WND_MSG_BEGINE + 5,	//����
		WM_WND_MSG_HELP_SRC		=	WM_WND_MSG_BEGINE + 6,	//����
		WM_WND_MSG_TEXT_MARK	=	WM_WND_MSG_BEGINE + 7,	//��ע
		WM_WND_MSG_ELEMENT		=	WM_WND_MSG_BEGINE + 8,	//ͨ��
		
		//begin fire
		WM_WND_MSG_BEGING_FOAM_FIRE = WM_WND_MSG_BEGINE +9,
		WM_WND_MSG_END_FOAM_FIRE    = WM_WND_MSG_BEGINE+10,
		WM_WND_MSG_FOAM_DATABALLMOVE = WM_WND_MSG_BEGINE + 11,
		WM_WND_MSG_BEGING_WARTER_FIRE = WM_WND_MSG_BEGINE +12,

		WM_WND_MSG_BEGINGPOOL_FIRE      =   WM_WND_MSG_BEGINE + 13,		//�ػ�
		WM_WND_MSG_BEGINGSPRAY_FIRE     =   WM_WND_MSG_BEGINE + 14,		//�����
		WM_WND_MSG_BEGINGEXPLOSION      =   WM_WND_MSG_BEGINE + 15,		//��ը
		
		//����
		Window_Message_BeginHotAreaEdit = WM_WND_MSG_BEGINE + 16,	//��ʼ�����༭
		Window_Message_DoubleHotMark	= WM_WND_MSG_BEGINE + 17,	//˫������ͼ��				
		Window_Message_ModifiyHotArea   = WM_WND_MSG_BEGINE + 18,    //��������
		Window_Message_EndModifiyHotArea = WM_WND_MSG_BEGINE + 19,	//�����������	
		Window_Message_CancelModifiyHotArea = WM_WND_MSG_BEGINE + 20, //ȡ�������༭
		Window_Message_EndingHotAreaEdit = WM_WND_MSG_BEGINE + 21,	//���������༭
		Window_Message_DeleteHotArea	= WM_WND_MSG_BEGINE + 22 ,	//ɾ������
		Window_Message_BeginDetectionHotArea = WM_WND_MSG_BEGINE + 23,	//�������
		Window_Message_DestroyPopMenu   =   WM_WND_MSG_BEGINE+ 24,		//���������Ҽ��˵�
		Window_Message_ClickPopMenu   =   WM_WND_MSG_BEGINE+ 25,			//����˵�
		Window_Message_WritedToDisk    = WM_WND_MSG_BEGINE+ 26,         //�Ѿ�д��Ӳ����

		//begin gas leak
		WM_WND_MSG_BEGINGASLEAK      =   WM_WND_MSG_BEGINE + 30,
		WM_WND_MSG_GASLEAK_DATABALLMOVE = WM_WND_MSG_BEGINE + 31,
		WM_WND_MSG_ENDGASLEAK		=  WM_WND_MSG_BEGINE + 32,
	
		//mark
		WM_WND_MSG_BEGINMARK		= WM_WND_MSG_BEGINE + 33,		//��ʼ���
		WM_WND_MSG_ENDMARK			= WM_WND_MSG_BEGINE + 34,			//�������
		WM_WND_MSG_ENDMARKLINE		= WM_WND_MSG_BEGINE + 35
	};
}

/***********************************************/
//�϶�ͼ���뿪���ں��͸���Ϣ
