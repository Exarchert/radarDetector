#pragma once
#include <osg/MatrixTransform>
#include <Object/Export.h>

namespace EMObj
{
	struct AssembleGroupDB
	{
	public:
		AssembleGroupDB()
		{
			position = osg::Vec3d();
			rotate = osg::Vec3();
			scale = osg::Vec3(1.0,1.0,1.0);
		}
		CString ID;
		osg::Vec3d position; //����γ�ȸ߶�
		osg::Vec3 rotate;    //yaw,pitch,roll
		osg::Vec3 scale;
	};

	//�����ṹ��
	struct MemberDB
	{
	public:
		MemberDB()
		{
			position = osg::Vec3();
			rotate = osg::Vec3();
			scale = osg::Vec3(1.0,1.0,1.0);
			m_thisNode = NULL;
		}

		CString strModelType;//"ģ��"  ����  "ԭ��"
		CString ID;//����ID
		CString assembleID;//��׼ID
		osg::Vec3 position;
		osg::Vec3 rotate;
		osg::Vec3 scale;
		std::string modelFilePath;
		osg::ref_ptr<osg::Node> m_thisNode;
	};


	class AssembleObjManager;
	//��׼��
class OBJECT_EXPORT AssembleObj:public osg::MatrixTransform
{
public:
	AssembleObj(void);
	AssembleObj(AssembleObjManager*);
	~AssembleObj(void);

	void init(AssembleGroupDB& assembelGroupDB);
	bool addMember(MemberDB& memberDB);
	bool GetObjectXYZ(CString& cstrID,osg::Vec3d& pos);//��ú�����������
	bool GetPartXYZ(CString& cstrObjectID,CString& cstrPartID,osg::Vec3d& pos);//��ú��ӵĲ�������������

	AssembleGroupDB& GetAssembleGroupDB(){return m_assembleGroupDB;}
	typedef std::map<CString,MemberDB> MapMemberDB;
	typedef std::map<CString,osg::ref_ptr<osg::MatrixTransform>> MapMemberNode;

	MapMemberDB& GetMapMemberDB(){return m_mapMemberDB;}
private:
	AssembleGroupDB m_assembleGroupDB;
	
	

	MapMemberDB m_mapMemberDB;
	MapMemberNode m_mapMemberNode;
	osg::ref_ptr<AssembleObjManager> m_pParent;
	
};

}