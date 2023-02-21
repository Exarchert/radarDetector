#pragma once
#include <COM/Ramble/BaseRamble.h>

namespace COM
{

typedef struct tagFreeSetting
{
	//	������ǰ�������ٶ�
	float fMoveAtSpd;
	//	���̣��������ǰ�������ٶ�
	float fMoveSpd;
	//	�ӽ���ת�ٶ�
	float fAngleSpd;
}FreeSetting;

class COM_EXPORT CFreeRamble : public CBaseRamble
{
public:
	CFreeRamble(osg::Camera* pCamera, osg::Node* pNode);
	~CFreeRamble(void);

	virtual const char* className() const { return "Free"; }

	virtual void setByMatrix(const osg::Matrixd& matrix);

	virtual void setByInverseMatrix(const osg::Matrixd& matrix);

	virtual osg::Matrixd getMatrix() const;

	virtual osg::Matrixd getInverseMatrix() const;

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	//	һ��ָ��
	void North();

	//	��ǰ�ƶ�
	void LevelForward(bool bForward = true);
	//	�����ƶ�
	void LevelLeft(bool bForward = true);

	void rotateYaw(bool bPositive = true);

	void rotatePitch(bool bPositive = true);

	void MoveAt(bool bPositive = true);

	void SetFreeSetting(FreeSetting& fs);
	void SetFreeSetting(CString strSetting);

	void GetFreeSetting(FreeSetting& fs);
private:
	void _keyDown( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &);
	void _keyUp( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter &);

	void flushMouseEventStack();

	void addMouseEvent(const osgGA::GUIEventAdapter& ea);

	bool isMouseMoving();

	bool calcMovement();

	bool getTerrainIntersection(osg::Camera *camera, osg::Node *subGraph, float x, float y, osg::Vec3 &pos);

	//	�ռ��ߵļн�
	float Line3DLine3DAngle(osg::Vec3d line1, osg::Vec3d line2, bool acuteAngleDesired);

	//	��line1��line2�Ƿ���˳ʱ����ת
	bool IsClockwise(osg::Vec2 line1, osg::Vec2 line2, bool &clockwise);

	//	��ǰ��תָ���Ƕ�
	void roundAhead(float angle);
	//	������תָ���Ƕ�
	void roundLeft(float angle);

	void MoveAtf(float fValue);

	void rotateYawf(float angle);

	void rotatePitchf(float angle);

	bool GetAngleAndAxis(float &fAngle, osg::Vec3 &v3RollAxis);

protected:
	//	�������
	osg::ref_ptr<osg::Camera>		m_Camera;
	//	����
	osg::ref_ptr<osg::Group>		m_earth;

	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;

	bool							_thrown;
	bool							_shift;
	bool							_ctrl;
	bool							_decelerateOffsetRate;
	bool							_straightenOffset;

	//	Ϊ����ת��������ת�Ǻ���ת�ᣬ��ֹƵ������
	float							m_fAngle;
	osg::Vec3						m_v3RollAxis;

	CRITICAL_SECTION				m_csFS;
	FreeSetting						m_FreeSetting;
};

}