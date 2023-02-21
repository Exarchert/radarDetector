/*************************************************
file:			COM_Object.h
Author:			��ƽ    
Date:			2009-11-18
Description:    �¼�����������
*************************************************/
#pragma once
namespace EMObj
{
	//��������Ϣ
	enum{
		Windows_Message_BeginEdit		= WM_USER,
		Windows_Message_EndEdit			= WM_USER + 1,
		Windows_Message_UpdateData		= WM_USER + 2,
		Windows_Message_ShowObjManager	= WM_USER + 3,
		Windows_Message_NewShowObj		= WM_USER + 4,		//�½���ʾ����
		Windows_Message_EditShowObj		= WM_USER + 5,		//�޸���ʾ����
		Windows_Message_AREAPOINTNOTICE = WM_USER + 6,		//�޸���ʾ����
		Windows_Message_MOUSEMOVE		= WM_USER + 7,		//�޸���ʾ����
		Windows_Message_SetDecoration	= WM_USER + 8,		//װ�ζ�����������
		Windows_Message_LONGPIPEDATADLG = WM_USER + 9,		//��������ߵ��빤��
		Windows_Message_SetWeather		= WM_USER + 10,		//��������Ч��
		Windows_Message_SetLighting		= WM_USER + 11,		//��������
		Windows_Message_EnterEdit		= WM_USER + 12,		//��ҵ��Ϣ�༭
		Windows_Message_UpdateBall		= WM_USER + 13,		//���²ɼ���
		Windows_Message_UpdateLod		= WM_USER + 14,		//����LOD�����еĲɼ���
		Windows_Message_ToggleFullScreen= WM_USER + 15,		//����ȫ���л�
		Windows_Message_MODELDATADLG	= WM_USER + 16,		//�첽����ģ�����ݵ��빤��
		Windows_Message_EnterpriseEdit	= WM_USER + 17,		//��ҵ�༭
		Windows_Message_LAYEREDIT		= WM_USER + 18,		//�������༭(ͼ��)
		Windows_Message_PopupMenu		= WM_USER + 19,		//�Ҽ��˵���Ϣ
		Windows_Message_MenuEnable		= WM_USER + 20,		//ϵͳ�˵�����
		Windows_Message_RambleSetting	= WM_USER + 21,		//��������
		Windows_Message_GUIEventAdapterPUSH=WM_USER+22,

		//��Ӷ���������Ϣ
		Window_Message_Play				= WM_USER + 100,
		Window_Message_Pause			= WM_USER + 101,
		Window_Message_Continue			= WM_USER + 102,
		Window_Message_Stop				= WM_USER + 103,
		Window_Message_Forward			= WM_USER + 104,
		Window_Message_Rewind			= WM_USER + 105,
		Window_Message_Flash_Play		= WM_USER + 106,	//Flash���Ŵ��ڵĴ���������
		Window_Message_Flash_Stop		= WM_USER + 107,
		Window_Message_Contrl			= WM_USER + 108,
		Window_Message_Seek				= WM_USER + 109,
		Window_Message_Play_Xml			= WM_USER + 110,	//xml�ڴ����ݲ��Ŷ���Ԫ�أ�liugh
		Window_Message_Second_Player	= WM_USER + 111,	//xml�ڴ����ݲ��Ŷ���Ԫ�أ�liugh



		//��ʾ����ѡ��仯��Ϣ
		Window_Message_SelChanged		= WM_USER + 200,
		Window_Message_ClearAreaPoints	= WM_USER + 201,
		Window_Message_SurveyTool		= WM_USER + 202,	//����
		Window_Message_ScreenTextMark	= WM_USER + 203,	//�û��Զ�����Ļ��ע
		Window_Message_GetPosByMouse	= WM_USER + 204,	//�û��Զ�����Ļ���ֱ�ע
		Window_Message_ScreenMarkImport = WM_USER + 205, 	//��Ļ��ע�������빤��
		Window_Message_ZipUnZipTool		= WM_USER + 206, 	//�ļ�ѹ�����ѹ����
		Window_Message_InterPpImportTool= WM_USER + 207,	//���ڹ��ߵ��빤��
		Window_Message_SaveDataDBToFile	= WM_USER + 208, 	//���ڹ��ߵ��빤��
		Window_Message_BatDelObj		= WM_USER + 209, 	//����ɾ������
		Window_Message_EnterBrowser		= WM_USER + 210,	//��ҵ�鿴
		Window_Message_Embankment       = WM_USER + 211,    //���
		Windows_Message_ProfileAnalysis = WM_USER + 212,     //�������
		Windows_Message_UpdateArea      = WM_USER + 213,      //
		Windows_Message_Streetscape     = WM_USER + 214,	  //�־�
		Windows_Message_QuickAddModel   = WM_USER + 215		  //���ټ���ģ��
	};
}
	