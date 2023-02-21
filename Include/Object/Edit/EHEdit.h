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

	//����UI�̣߳�CDlgEditModel::Update(WPARAM Para1=0, WPARAM Para2=0)�����ᱻ����
	void UpdateUI(DWORD Para1 = 0, DWORD Para2 = 0);

	/*
	Ŀ�ģ�������ʾ��(������ģ��)����ʾ��ǰ��λ�ú���̬
	������szHint		(in)		��Դ����
	*/
	void SetHintBall(LPCTSTR szHint);

	//��ȡ��ʾ��Ľ��
	osg::Node * GetHintBall() const;

	//������ʾ��λ�ú���̬
	void UpdateHintBall(const osg::Matrixd &mat);

	//�����ʾ��
	void AddHintBall();

	//�Ƴ���ʾ��
	void RemoveHintBall();

	CEditPage * GetEditPage(){ return m_pEditPage;};
protected:
	EMObj::CObj* m_pObj;
	CEditPage * m_pEditPage;

	//��ʾ��(������ģ��)
	osg::ref_ptr<osg::MatrixTransform>	m_trans;	//�任����
};

//֪ͨ��Ⱦ�߳������¼�����
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