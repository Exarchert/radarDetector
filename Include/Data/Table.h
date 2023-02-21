/*************************************************
file:			COM_DataTable.h
Author:			��ƽ    
Date:			2009-03-18
Description:    �ڴ��
*************************************************/
#pragma once
#include <COM\loaddown\LoadDown.h>
#include <Data\Export.h>
#include <vector>
#include <map>
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <Data\Row.h>

//#import "C:\\Program Files\\Common Files\\system\\ado\\msadox.dll"
//#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" rename("EOF","ADOEOF")
//
//using namespace ADODB;


namespace Data
{

template<class T>
class CTable : public osg::Referenced
{
public:
	CTable(void)
	{
		m_Count = 0;
		m_IsFirstRead=false;
	}
	virtual ~CTable(void){}

	bool Add(T* value)
	{
		if(value == NULL)
			return false;
		if(m_mapTable.find(value->ID) != m_mapTable.end())
			return false;
		m_mapTable[value->ID] = value;
		m_Table.push_back(value);
		m_Count++;
		return true;
	}

	bool Del(T* value)
	{
		return Del(value->ID);
	}

	bool Del(LONG64 ID)
	{
		std::vector<osg::ref_ptr<T>>::iterator vItr;
		for(vItr = m_Table.begin(); vItr != m_Table.end(); vItr++)
		{
			if((*vItr)->ID == ID)
			{
				m_Table.erase(vItr);
				break;
			}
		}

		std::map<LONG64, osg::ref_ptr<T>>::iterator mItr;
		mItr = m_mapTable.find(ID);
		if(mItr == m_mapTable.end())
		{
			return false;
		}

		m_mapTable.erase(mItr);
		m_Count--;
		return true;
	}

	void Clear()
	{
		m_Table.clear();
		m_mapTable.clear();
		m_Count = 0;
	}

	bool WriteFile(const CString& ExeResPath)
	{
		try
		{
			CString  fileName = ExeResPath+CString(_T("\\Data\\"))+ T::m_ViewName +CString(_T(".dpf"));
			std::ofstream  fileForWrite(fileName,std::ios::binary|std::ios::trunc);
			if (!fileForWrite.is_open())
			{
				return false;
			}

			fileForWrite.seekp(0,std::ios::beg);

			std::vector<osg::ref_ptr<T>>::iterator vItr;

			for(vItr = m_Table.begin(); vItr != m_Table.end(); vItr++)
			{

				(*vItr)->WriteFile(fileForWrite);
			}
			fileForWrite.close();
			return true;
		}
		catch(_com_error e)
		{
			//TRACE0((LPCTSTR)e.Description());
			return false;
		}
		

	}
	bool ReadFromFile(const CString& ExeResPath)
	{
		CString  fileName = ExeResPath+CString(_T("\\Data\\"))+T::m_ViewName +CString(_T(".dpf"));
		return ReadFile(fileName);
	}
	bool ReadFile(const CString& fileName)
	{
		try
		{
			std::ifstream fileForRead(fileName,std::ios::binary);
			if (!fileForRead.is_open())
			{
				return false;
			}
			fileForRead.seekg(0,std::ios::beg);
			// ��ȡ������row
			// ��row�����ļ���������row��read������ȡ���ƶ��ļ�ָ�롣
			// Ϊ��ʹrow�Ĳ���Ӱ�쵽�ļ�����ʹ�����ô�ֵ
			fileForRead.peek();
			while (!fileForRead.eof())
			{
				osg::ref_ptr<T> value = new T;
				if (value->ReadFile(fileForRead))
				{
					m_Table.push_back(value);
					m_mapTable[value->ID] = value;
					++m_Count;
				}
				fileForRead.peek();
			}
			fileForRead.close();
			return true;
		}
		catch(_com_error e)
		{
			return false;
		}
	}
	bool ReadFromWebFile(const CString& Url, const CString& CachePath)
	{
		try
		{
			CString FilePath;
			if (Url.Find(_T('/')) == -1 )
			{			
				FilePath = Url+CString(_T("\\Data\\"))+T::m_ViewName +CString(_T(".dpf"));
			}
			else
			{
				CString UrlFile = Url+CString(_T("/Data/"))+T::m_ViewName +CString(_T(".dpf"));
				if (COM::CLoadDown::Read(UrlFile, CachePath, FilePath) == false)
				{
					return false;
				}
			}
			return ReadFile(FilePath);
		}
		catch(_com_error e)
		{
			return false;
		}

	}

	bool Load(_ConnectionPtr pConn)
	{
		try
		{
			_RecordsetPtr rst(__uuidof(Recordset));

			rst->Open((LPCTSTR)T::m_ViewName,_variant_t((IDispatch*)pConn),
				adOpenStatic, adLockReadOnly, adCmdUnknown);
			while(!rst->ADOEOF)
			{
				osg::ref_ptr<T> value = new T;	
				if(value->Read(rst))
				{
					m_mapTable[value->ID] = value;
					m_Table.push_back(value);
					m_Count++;
				}
				rst->MoveNext();

			}
			rst->Close();
			rst.Release();
		}
		catch(_com_error e)
		{
			CString strMsg = e.Description();
			return false;
		}
		return true;

	}

	void ReLoad()
	{
		m_Count = 0;
		Clear();
		Load();
	}
	
	T* GetRow(LONG64 ID)
	{
		if(m_mapTable.find(ID) == m_mapTable.end())
			NULL;
		return m_mapTable[ID].get();
	}

	T* GetRowByIndex(int Index)
	{
		if(Index < 0 || Index >= (int)m_Table.size())
			NULL;
		return m_Table[Index].get();
	}

	int GetCount()
	{
		return m_Count;
	}
protected:
	std::vector<osg::ref_ptr<T>> m_Table;
	std::map<LONG64, osg::ref_ptr<T>> m_mapTable;
	int m_Count;
	bool  m_IsFirstRead;
};

//Ĭ�����ͱ�
typedef CTable<CRow> CTableDef;
}