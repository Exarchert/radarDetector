#pragma once

namespace Kernel
{//�洢�����ڵ�ģ�ͣ������ڳ�����ɾ������
	class AnimationDotNodeDB
	{
	public:
		int m_ID;
		osg::ref_ptr<osg::Node> m_animationDotNode;
	};

	//�洢�����ڵ�ģ�͵�����ģ��
	class AnimationDotChildMode
	{
	public:
		//double m_modeID;
		std::vector<CString> m_childModeID;
	};

	//�洢���������ܵ�ģ�ͽڵ����������
	class AnimationPipleLineModeDB
	{
	public:
		AnimationPipleLineModeDB()
		{
			m_beginModelID = L"";
			m_radius = 0.1;
			m_speed = 1;

		}
		CString m_beginModelID;
		std::map<CString,AnimationDotChildMode> m_animationPipleLine;//���߽ڵ�
		float m_radius;											   //�뾶	 		
		float m_speed;											   //�ٶ� 	
	};
}