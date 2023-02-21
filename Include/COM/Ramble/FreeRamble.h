#pragma once
#include <COM/Ramble/BaseRamble.h>

namespace COM
{

typedef struct tagFreeSetting
{
	//	鼠标滚轮前进后退速度
	float fMoveAtSpd;
	//	键盘，控制面板前后左右速度
	float fMoveSpd;
	//	视角旋转速度
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

	//	一键指北
	void North();

	//	向前移动
	void LevelForward(bool bForward = true);
	//	向左移动
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

	//	空间线的夹角
	float Line3DLine3DAngle(osg::Vec3d line1, osg::Vec3d line2, bool acuteAngleDesired);

	//	由line1到line2是否是顺时针旋转
	bool IsClockwise(osg::Vec2 line1, osg::Vec2 line2, bool &clockwise);

	//	向前旋转指定角度
	void roundAhead(float angle);
	//	向左旋转指定角度
	void roundLeft(float angle);

	void MoveAtf(float fValue);

	void rotateYawf(float angle);

	void rotatePitchf(float angle);

	bool GetAngleAndAxis(float &fAngle, osg::Vec3 &v3RollAxis);

protected:
	//	场景相机
	osg::ref_ptr<osg::Camera>		m_Camera;
	//	地球
	osg::ref_ptr<osg::Group>		m_earth;

	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;

	bool							_thrown;
	bool							_shift;
	bool							_ctrl;
	bool							_decelerateOffsetRate;
	bool							_straightenOffset;

	//	为自旋转，保存旋转角和旋转轴，防止频繁计算
	float							m_fAngle;
	osg::Vec3						m_v3RollAxis;

	CRITICAL_SECTION				m_csFS;
	FreeSetting						m_FreeSetting;
};

}