#pragma once

namespace Kernel
{//存储动画节点模型，用于在场景中删除动画
	class AnimationDotNodeDB
	{
	public:
		int m_ID;
		osg::ref_ptr<osg::Node> m_animationDotNode;
	};

	//存储动画节点模型的子类模型
	class AnimationDotChildMode
	{
	public:
		//double m_modeID;
		std::vector<CString> m_childModeID;
	};

	//存储整条动画管道模型节点的所有数据
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
		std::map<CString,AnimationDotChildMode> m_animationPipleLine;//管线节点
		float m_radius;											   //半径	 		
		float m_speed;											   //速度 	
	};
}