#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>
#include <CartoonObject\DeskManoeuver\DisasterSource\PoolFireEffect.h>
#include <CartoonObject\DeskManoeuver\DisasterSource\ExplosionEffect.h>
#include <CartoonObject\DeskManoeuver\DisasterSource\SprayFireEffect.h>

namespace CartoonObject
{
	class CDisaster : public CDeskManoElements
	{
	public:
		CDisaster(void);
		~CDisaster(void);

		//�ֺ�����
		enum DISASTERTYPE
		{
			Explosion = 0,	//��ը
			Pool_Fire,		//�ػ�
			Spray_Fire,		//�����
			Leak_Air		//����й¶
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

		DWORD GetDisasterType()
		{
			return m_DisasterTepy;
		}

		void SetDisasterType(DWORD type);

	public:
		//��ά�̵߳���
		void CreatePoolFire(osg::Vec3d pos);
		void CreateSprayFire(osg::Vec3d pos);
		void CreateExplosion(osg::Vec3d pos);

		void UpdatePoolFire(const osg::Matrixd &mat);
		void UpdatePoolFire(float widht,float hight,float size);

		void UpdateSprayFire(const osg::Matrixd &mat);
		void UpdateSprayFire(float indensity);

		void UpdateExplosion(const osg::Matrixd &mat);
		void UpdateExplosion(float indensity);

		bool FinishDisaster(CString strFilePath,DWORD type);
		void RemoveDisaster(DWORD type);

		osg::Matrixd GetDisasterMat();
	private:
		osg::ref_ptr<CartoonObject::CPoolFireEffect>	m_PoolFire;
		osg::ref_ptr<CartoonObject::CSprayFireEffect>	m_SprayFire;
		osg::ref_ptr<CartoonObject::CExplosionEffect>	m_Explosion;

		osg::Matrixd									_DisasterMatrix;
	private: 
		DWORD m_DisasterTepy;
	};
}
