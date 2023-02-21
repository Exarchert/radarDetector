/*************************************************
file:			ParticleManager.h
Author:			xubaolong    
Date:			2013-09-05
Description:    ����ϵͳ����
*************************************************/
#pragma once
#include <Object/Export.h>

#include <osg/Referenced>
#include <osgParticle/ParticleEffect>
#include <vector>
#include <Object\ConduitParticle.h>//#define OUTPUTMODELS

namespace EMObj
{

class OBJECT_EXPORT ParticleManager : public osg::Referenced
{
public:
	ParticleManager(void);
	~ParticleManager(void);

	void Init(osg::Group *particleManager);

	void addConduitEffect(CString id,osg::Vec3 pos);
	void addConduitEffect2(CString id,osg::Vec3d pos,CString stype ,CString strParam);
	void deleteEffect(CString id);

	
protected:
	typedef std::map<CString, osg::MatrixTransform*> MapParticleEffect;
	MapParticleEffect m_mapParticleEffect;

	osg::ref_ptr<osg::Group> m_particleManager;
	osg::ref_ptr<EMObj::ConduitEffect> m_conduitEffect;

#if defined OUTPUTMODELS
	std::map<__int64, osg::Matrix>	_enterMatrixList;
#endif
	//
};

//XML��ȡ�ͱ���ģ��
OBJECT_EXPORT ParticleManager *GetParticleManager();
}