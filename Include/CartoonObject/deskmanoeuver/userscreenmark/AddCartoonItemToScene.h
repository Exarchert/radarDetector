
/*************************************************************************/
//FileName：CAddCartoonItemToScene.h CAddCartoonItemToScene.cpp
//Author:	Liugh
//Date：2009-10-26
//Description：Add a text node to 3d scene in per frame
/**************************************************************************/
#include <osgGA/GuiEventHandler>
#include <osgText/Text>
#include <osg/Geode>
#pragma once

class CAddCartoonItemToScene :	public osgGA::GUIEventHandler
{
public:
	CAddCartoonItemToScene(void);
	~CAddCartoonItemToScene(void);

	static CAddCartoonItemToScene* GetInstance(void)
	{
		if(!_pHandler)
			_pHandler = new CAddCartoonItemToScene();
		return _pHandler;
	}

	static void Release();
	static osg::ref_ptr<CAddCartoonItemToScene> _pHandler;


public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	void AddMarkNodeToScene(osg::Node* node);

	void RemoveMarkNodeFromScene(osg::Node* node);

	void RemoveAll();

	bool SelectCheck(float x,float y);
	void HighlightText(bool bHighLight);

public:
	osg::ref_ptr<osg::Node> m_textNode;
	osg::ref_ptr<osg::Node> m_preTextNode;
	bool m_bAddTextNode;
	bool m_bRemoveTextNode;
	bool m_bAddMarkNode;
	bool m_bRemoveMarkNode;

	osg::ref_ptr<osg::Switch> m_WndNode;
	osg::ref_ptr<osg::Switch> m_preWndNode;
	//单个文字的添加与删除
	bool m_bAddWndNode;
	bool m_bRemoveWndNode;
	int	 m_wndId;
	int  m_preWndId;
	//批量添加与删除文字
	std::vector<osg::Node*> m_AddTextNodeVec;
	std::vector<osg::Node*> m_RemoveTextNodeVec;
	std::vector<osg::Switch*> m_RemoveNodeVec;
	std::vector<osg::Switch*> m_AddNodeVec;

	//屏幕标注文字属性参数
	bool _bUpdateText;
	BOOL _bUseWnd;
	CString _textContent;
	CString _fontName; 
	osg::Vec4 _color;
	float _size;
	float _xOrigion;
	float _yOrigion;
	HWND  _TextMarkHandle;
	BOOL  _bGetPos;
	osgText::Text* _Text;
	osg::Geode* _wndGeode;
	osg::Vec4 _bkColor;
	
	std::map<int,osg::Group*> _TextMarkMap;

	void UpdateScreenTextMark();

	void addMarkNodeToMap(osg::Group* wndRoot);

	//remove the last mark node
	void removeMarkNodeFromMap();
	
	void SetActive(BOOL bActive){m_bActive = bActive;}

private:
	bool	m_bPick;
	BOOL	m_bActive;
};
