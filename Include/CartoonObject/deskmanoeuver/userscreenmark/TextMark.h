
/*************************************************************************/
//	FileName��CTextMark.h CTextMark.cpp
//	Author:	Liugh
//	Date��2009-10-26
//	Description��Text manager
/**************************************************************************/

#pragma once
#include "AddCartoonItemToScene.h"
#include <CartoonObject/DeskManoeuver/DeskManoElements.h>

using namespace CartoonObject;

class CTextMark : public CartoonObject::CDeskManoElements
{
public:
	CTextMark(void);
	//��ӵ��ű�
	CTextMark(int mode);

	~CTextMark(void);
	
 	struct TextStruct
 	{
 		int			_textId;		//����Id
 		float		_textSize;		//���ִ�С
 		osg::Vec4	_textColor;		//��ɫ
 		CString		_textContent;	//��ע����
		int			_fontIndex;		//����ѡ������
 		CString		_textFont;		//����
 		float		_xScreen;		//��������x����
 		float		_yScreen;		//��������y����
		osgText::Text* _Text;		//����ָ��
 	};
public:

	virtual CString GetName() const;
	virtual void SetName(CString strName);

	virtual UINT GetType() const;

	virtual float GetStartTime() const;
	virtual	void SetStartTime(float tm);

	virtual float GetLifeTime() const;
	virtual void SetLifeTime(float life);

	virtual CString GetInsruction() const;
	virtual	void SetInstruction(CString strInfo);

public:
	void AddMarkNodeToScene(osg::Node* node);

	void RemoveMarkNodeFromScene(osg::Node* node);

	void SetHandle(HWND handle);

	void BootMouseGetPos(BOOL bGetPos);

	
	//Description��Create Text Pointer
	//Param Explain: 
	bool CreateScreenTextMark(CString &textContent, CString &fontName, osg::Vec4 color, float size,
		osgText::Text *&outText,const float& xOrigion,const float& yOrigion);

	/*Description��Create Text Node combination with text pointer*/
	osg::Node* CreateTextNode(osgText::Text *&outText);

	void UpdateText(CString &textContent, CString &fontName, osg::Vec4 color, float size,
		osgText::Text *&outText,const float& xOrigion,const float& yOrigion);

	void ResetWnd(BOOL resetWnd,osg::Geode* wndGeod,osg::Vec4 color);

	bool CreateTextBkWnd(float textMinX,float textMinY,float textMaxX, float textMaxY,osg::Vec4& bkColor,osg::Geode* wndGeode);

	void clearTextMark();

	void AddWndToScene(osg::Switch* node);

	void RemoveWndFromScene(osg::Switch* node);

	void SetActive(BOOL bActive);

public:
	osg::ref_ptr<CAddCartoonItemToScene> m_handler;

	static bool GetAllFonts(std::map<CString, CString> &Fonts);
};