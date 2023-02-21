/***********************************************************************
Copyright (C),  
�ļ���TEMPExports.h TEMPExports.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-02-02
����������ģ��ĵ����ӿ�.

�޸ļ�¼��
����						����					����
-----------------------------------------------------------------------
liugh	add function	forward rewide seek SetScriptPath GetScriptPath
						PlayScriptFromXmlString GetScriptTimeLength
***********************************************************************/

#pragma once

#include <temp/Export.h>
#include <com/KernelContext/KernelContext.h>
#include <object/ObjManager.h>


namespace TEMP		{

/*
Ŀ�ģ���ʼ������ģ��.
������szBinPath		(in)	��������Ŀ¼
	  pContext		(in)	�ں�����������ָ��
	  pObjMan		(in)	����������ָ��
*/
TEMP_EXPORT void Init(const CString &szBinPath, COM::CKernelContext *pContext, EMObj::CObjManager *pObjMan);

//������ģ��
TEMP_EXPORT void UnInit();

/*
Ŀ�ģ����Žű�
������szScript		(in)	�ű�����

������������������������������������������������������������������������������

Liugh��Ӳ��� mode	(in)	�ļ����ط�ʽ��Ĭ��Ϊ(mode=0)archive�ļ�������Ϊ(mode=1)xml��ʽ�ļ�
*/
TEMP_EXPORT void PlayScript(const CString &szScript);

TEMP_EXPORT void PlayNewScript(const CString &szScript);



/*
purpose:	Load Animation data from xml string
parameter:	xmlString	(in)	description of an action for animation item
Liugh��� 	
*/

/*
<camera name="camera" b_t="0" l_t="40">
	<tracks>
		<track timelen="16.5">
			<pos bx="-2437113.7268576822" by="5364402.4324997868" bz="2434623.2714401949"  ex="-2437029.9309432711" ey="5362686.4436971657" ez="2439277.1751346760" ></pos>
			<pose bpx="-1.9400000572204601" bpy="-7.9099993705749503" bpz="0" epx="-1.9400000572204601" epy="-7.9099993705749503" epz="0"></pose>
			<speed v0="300" a="0" ></speed>
		</track>
	</tracks>
</camera>
*/

TEMP_EXPORT void PlayScriptFromXmlString(const CString &xmlString);

/*
Ŀ�ģ���ͣ\����
������bPause	(in)	true:��ͣ, false:����
*/
TEMP_EXPORT void Pause(bool bPause);

//ֹͣ
TEMP_EXPORT void Stop();

//���
TEMP_EXPORT void Forward(int step);

//����
TEMP_EXPORT void Rewind(int step);

//��ת
TEMP_EXPORT void Seek(double time);

//������Ŀ¼
TEMP_EXPORT void SetScriptPath(CString ScriptPath);

TEMP_EXPORT CString GetScriptPath(CString& ScriptPath = CString(_T("")));

/*
��ȡ�����õ�ǰ���ڲ��ŵĽű�����
*/
TEMP_EXPORT CString GetCurScriptName(CString& ScriptPath = CString(_T("")));
TEMP_EXPORT void SetCurScriptName(CString ScriptPath);

//��ȡ���Žű�ʱ�䳤��
TEMP_EXPORT double GetScriptTimeLength();

//��ȡ����ָ���ű��Ľű�ʱ�䳤��
TEMP_EXPORT double GetScriptTimeLength(CString scriptFullPath);

//��ȡ��ǰ����ʱ��
TEMP_EXPORT double GetCurPlayTime();


extern TEMP_EXPORT CString g_AnimationPath;
extern TEMP_EXPORT CString g_CurPlayScriptName;
extern TEMP_EXPORT BOOL g_AnimationIsFinish;
}