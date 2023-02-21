/*************************************************************************
File:			EventDef.h
Author:			��ƽ    
Date:			2009-0x00000010-18
Description:    �¼�����ͷ�ļ�


*************************************************************************/
#pragma once

//�������
#define USER_MSG_START						(0x0400 + 100)
#define CTRL_PANEL_FORWARD					(USER_MSG_START+1)
#define CTRL_PANEL_BACK						(USER_MSG_START+2)
#define CTRL_PANEL_LEFT						(USER_MSG_START+3)
#define CTRL_PANEL_RIGHT					(USER_MSG_START+4)
#define CTRL_PANEL_ROTATELEFT				(USER_MSG_START+5)
#define CTRL_PANEL_ROTATERIGHT				(USER_MSG_START+6)
#define CTRL_PANEL_CAMERA_FORWARD			(USER_MSG_START+7)
#define CTRL_PANEL_CAMERA_BACK				(USER_MSG_START+8)


//�¼�����
#define Event_Base						0x00000001		//�����¼�
#define Event_KeyboardAndMouse			0x00000002		//���ͼ����¼�
#define Event_OperatorsRefresh			0x00000004		//���ͼ���ˢ���¼�
#define Event_CommonComponent			0x00000008		//��������¼�
#define Event_SysShowObj_Base			0x00000010		//������ʾ��������¼�
#define Event_SysShowObj_Operation		0x00000020		//������ʾ��������¼�
#define Event_SysShowObj_Control 		0x00000040		//������ʾ��������¼�
#define Event_AppShowObj_Base			0x00000080		//�û���ʾ��������¼�
#define Event_AppShowObj_Operation		0x00000100		//�û���ʾ��������¼�
#define Event_AppShowObj_Control		0x00000200		//�û���ʾ��������¼�
#define Event_Measurement				0x00000400		//�����¼�
#define Event_RegionSelect              0x00001600      //��ѡ�¼� 
#define Event_XmlTabel                  0x00003200		//�����¼�
#define Event_PipleObjSelect            0x00003201		//����ѡ��
#define Event_ClickMessage              0x00003202      //����¼� 


//�����¼���Ϣ
#define Message_Base_InitSysModule		0x00000001		//��ʼ��ϵͳģ��
#define Message_Base_InitAppModule		0x00000002		//��ʼ��Ӧ��ģ��
#define Message_Base_StartRender		0x00000004		//��ʼ��Ⱦ
#define Message_Base_UnInitAppModule	0x00000008		//����ʼ��Ӧ��ģ��
#define Message_Base_UnInitSysModule	0x00000010		//����ʼ��ϵͳģ��
#define Message_Base_StopRender			0x00000020		//ֹͣ��Ⱦ
#define Message_Base_InitFailure		0x00000040		//��ʼ��ʧ��

//���ͼ����¼���Ϣ
#define Message_KM_PUSH					0x00000001		//��갴���¼�
#define Message_KM_RELEASE 				0x00000002		//��굯���¼�
#define Message_KM_DOUBLECLICK			0x00000004		//���˫���¼�
#define Message_KM_DRAG					0x00000008		//�϶��¼�
#define Message_KM_MOVE					0x00000010		//����ƶ�
#define Message_KM_KEYDOWN				0x00000020		//���̰���
#define Message_KM_KEYUP				0x00000040		//����̧��
#define Message_KM_RESIZE				0x00000100		//��С�ı�
#define Message_KM_SCROLL				0x00000200		//����
#define Message_KM_KSDOWN_MRIGHTRELEASE	0x00000400		//���̣Ӱ��²�������Ҽ�̧���¼�

//������ˢ���¼�
#define Message_OperatorsRefresh_Update 0x00000001		//������ˢ��	

//��������¼���Ϣ

//��������¼���Ϣ
#define Message_ShowObj_Base_Init		0x00000001		//��ʼ��
#define Message_ShowObj_Base_Draw		0x00000002		//����
#define Message_ShowObj_Base_Refresh	0x00000004		//ˢ��
#define Message_ShowObj_Base_Locate		0x00000008		//��λ
#define Message_ShowObj_Base_BeginEdit	0x00000010		//��ʼ�༭
#define Message_ShowObj_Base_EndEdit	0x00000020		//�����༭
#define Message_ShowObj_Base_AddToDB	0x00000021		//�ɹ����һ�����ݵ����ݿ�
#define Message_ShowObj_Base_UptadeToDB	0x00000022		//�ɹ��޸�һ�����ݵ����ݿ�
#define Message_ShowObj_Base_DeleteToDB	0x00000023		//�ɹ�ɾ��һ�����ݴ����ݿ�
#define Message_ShowObj_Base_ActionDef	0x00000040		//��������
#define Message_ShowObj_Base_SaveToDB	0x00000080		//�洢�����ݿ�
#define Message_ShowObj_Base_Selected	0x00000100		//ѡ��




//��������¼���Ϣ


//����ˢ���¼���Ϣ
#define Message_ShowObj_Control_IsLodAble		0x00000001		//Lod�����Ƿ���Ч

