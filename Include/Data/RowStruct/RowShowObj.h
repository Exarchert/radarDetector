/***************************************************************
�޸ļ�¼��
����                      ����                 ����
----------------------------------------------------------------
��־��                 2009-06-04            ��д��Ա����Add()
л��ҫ                 2009-11-03            ���m_Lod��Ա��Read������Ӷ�ȡLod�ֶε�����
***************************************************************/

#pragma once
#include <Data/Export.h>
#include <osg\Vec3d>
#include <Data\Row.h>
#include <fstream>

namespace Data{

class DATA_EXPORT CRowShowObj : public CRow
{
public:
	CRowShowObj(void);
	~CRowShowObj(void);

	int		m_EnterID;				//��ҵID
	CString m_Name;					//����
	LONG64  m_TypeID;				//����ID
	LONG64	m_LodConID;				//LOD����ID
	CString m_EyePosAndPose;		//�۲��λ�ú���̬
	int		m_Visable;				//����
	int		m_ShowPriority;			//��ʾ���ȼ�
	CString	m_Property;				//XML����
	LONG64  m_ParentID;				//���ڵ�
	CString	m_LodPara;				//Lod����

	CString Add();
	CString Update();
	bool Read(_RecordsetPtr rst);
	bool ReadFile(std::ifstream& tableFile);
	bool WriteFile(std::ofstream& tableFile);
	CString GetTableName();
	CString GetViewName();

	static CString m_TableName;
	static CString m_ViewName;

public:
	static int m_NameLen;      // ��ҵ���Ƶ��ַ���
	static int m_LodParaLen;	//lod��������

	enum{
		RowType = 0x00000004
	};

	virtual DWORD GetType(){return RowType;}
};
}