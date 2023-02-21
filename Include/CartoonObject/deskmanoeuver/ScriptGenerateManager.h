#pragma once
#include <osg/Referenced>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonUnit.h>
#include <CartoonObject/DeskManoeuver/DeskManoElements.h>

namespace CartoonObject
{
	class CScriptGenerateManager : public osg::Referenced
	{
	public:
		CScriptGenerateManager (void);
		~CScriptGenerateManager (void);

		void AddElement(CDeskManoElements* pElement);
		void RemoveElement(UINT index);
		void InsertElement(UINT index,CDeskManoElements* pElement);
		void ClearElements();

		void GenerateScript(CString filePath);
		void AddElementToCartUnit(CDeskManoElements* pElement,CCOM_CartoonUnit* pCartUnit);
	public:
		std::vector<osg::ref_ptr<CDeskManoElements>>	m_ElementList;
	};
}