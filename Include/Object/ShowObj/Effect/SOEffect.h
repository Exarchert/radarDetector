/***************************************************************************
Copyright (C),  
�ļ���SOEffect.h
���ߣ�л��ҫ
�汾��0.1
���ڣ�2010-03-24
������Ч���࣬�������Ӻ�shader��һЩЧ��

�޸ļ�¼��
����						����					����
***************************************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osgDB\ReadFile>
#include <osgAPEx/BaseGeo>
#include <osgParticle/ParticleSystem>
#include <osgParticle/Particle>
#include <osgParticle/FluidProgram>
#include <osgParticle/FluidFrictionOperator>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/SectorPlacer>
#include <osgParticle/ModularProgram>
#include <osgParticle/AccelOperator>
#include <osg/TexEnvCombine>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osgAPEx/CoordinateTool>
#include <osgAPEx/RadialShooter>
#include <osgAPEx/ParticleEffects>

namespace EMObj{

class OBJECT_EXPORT CSOEffect : public EMObj::CObj
{
public:
	CSOEffect(void);
	~CSOEffect(void);

	enum
	{
		SOType = 19
	};

	DWORD GetType()
	{
		return SOType;
	}

	DWORD GetRenderType()
	{
		return Render_Type_Scene;
	}

	//////////////////////////////////////////////////////////////////////////
	//��дCObj��Ϊ
	//��ȡ������Ϣ
	bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetProperty();

	//ˢ�»���
	virtual void UpdateDraw(DWORD Value = 0);

	//�����༭�Ի���
	bool BeginEditUI();

	//�����༭
	bool EndEdit3D();

	//��дʰȡ
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	/*
	Ŀ�ģ�����XML�ڵ��е�����Ч�����ݲ����������������
	������pNode		    (in)	XML�ڵ�
	      lifeTime		(out)	��������
	      size		    (out)	���Ӵ�С
	      rate		    (out)	��������
	      dir		    (out)	����
	      speed		    (out)	�ٶ�
		  theta		    (out)	��ֱ��Χ
	���أ��ɹ�����true�����򷵻�false
	*/
	bool ParseParticleEffectXml(MSXML2::IXMLDOMNodePtr pNode,double& lifeTime,osg::Vec2& size,osg::Vec2& rate,
		                        osg::Vec3& dir,osg::Vec2& speed,osg::Vec2& theta);

	/*
	Ŀ�ģ�����XML�ڵ��е�shader����Ч�����ݲ����������������
	������pNode		                (in)	XML�ڵ�
	      particleSystemHeight		(out)	����ϵͳ�߶�
	      particleSpeed     		(out)	ȼ���ٶ�
	      particleSpread     		(out)	������
	      particleSystemShape     	(out)	����Ƭ���
		  particleShape         	(out)	������״
		  particleSize          	(out)	���Ӵ�С
	���أ��ɹ�����true�����򷵻�false
	*/
	bool ParseShaderFireXml(MSXML2::IXMLDOMNodePtr pNode,float& particleSystemHeight, float& particleSpeed,
		                    float& particleSpread,float& particleSystemShape,float& particleShape,float& particleSize);

	//�������е�����ΪĬ��
	void SetInitValue();

private:
	//��������Ч���ڵ�
	osg::Node* CreateParticleNode();
	//�������ӻ���Ч��
	osg::Node* CreateShaderFireNode();

	//��ȡ�ڵ��е�����
	bool ParseXml(MSXML2::IXMLDOMNodePtr pNode);
	//��ȡ�ڵ��е�ģ������
	bool ParseTemplateParaXml(MSXML2::IXMLDOMNodePtr pNode);

public:
	bool       m_useTemplate;
	int        m_templateID;

	osg::Vec3d m_centerLLA;
    osg::Vec3f m_centerYPR;
    osg::Vec3d m_desLLA;

    CString m_effectType;
	CString m_imagePath;
	CString m_modelPath;

	double m_particleLifeTime;
	osg::Vec2 m_particleSizeRange;
	osg::Vec2 m_rateCounterRange;

	osg::Vec3 m_shooterDir;
	osg::Vec2 m_shooterSpeedRange;
	osg::Vec2 m_shooterThetaRange;

public:
    float    m_particleSystemHeight;
    float    m_particleSpeed;
	float    m_particleSpread;
	float    m_particleSystemShape;
	float    m_particleShape;
	float    m_particleSize;

private:
	osg::ref_ptr<osg::Geode>                m_axisNode;
	osg::ref_ptr<osg::MatrixTransform>      m_axisTransform;
	osg::ref_ptr<osg::Group>                m_effectRoot;
	osg::Node*                              m_effectNode;
};

}
