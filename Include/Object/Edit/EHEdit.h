#pragma once
#include <Object\Export.h>
#include <object\Edit\EditPage.h>
#include <osgGA\GUIEventHandler>
#include <osg\MatrixTransform>
#include <osg\Matrixd>
#include <object\Obj.h>
#include <Com\command\Cmd.h>

#pragma once


namespace EMObj{
class OBJECT_EXPORT CEHEdit : public osgGA::GUIEventHandler
{
public:
	CEHEdit(void);
	~CEHEdit(void);

	void SetObjAndDlg(CObj *pObj, CEditPage *pEditPage);

	//更新UI线程，CDlgEditModel::Update(WPARAM Para1=0, WPARAM Para2=0)方法会被调用
	void UpdateUI(DWORD Para1 = 0, DWORD Para2 = 0);

	/*
	目的：设置提示球(或其它模型)，表示当前的位置和姿态
	参数：szHint		(in)		资源名称
	*/
	void SetHintBall(LPCTSTR szHint);

	//获取提示球的结点
	osg::Node * GetHintBall() const;

	//更新提示球位置和姿态
	void UpdateHintBall(const osg::Matrixd &mat);

	//添加提示球
	void AddHintBall();

	//移除提示球
	void RemoveHintBall();

	CEditPage * GetEditPage(){ return m_pEditPage;};
protected:
	EMObj::CObj* m_pObj;
	CEditPage * m_pEditPage;

	//提示球(或其它模型)
	osg::ref_ptr<osg::MatrixTransform>	m_trans;	//变换矩阵
};

//通知渲染线程增加事件监听
class CCmdAddEventHandler : public COM::CCmdObj
{
public:
	CCmdAddEventHandler( EMObj::CEHEdit* pEHEdit)
	{
		m_pEHEdit = pEHEdit;
	}
	void DoAction()
	{
		if (m_pEHEdit) {
			m_pEHEdit->AddHintBall();
			g_pKC->AddEventHandler(m_pEHEdit);
		}
	}
protected:
	EMObj::CEHEdit* m_pEHEdit;
};

}