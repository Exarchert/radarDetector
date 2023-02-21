#pragma once
#include <osg/Switch>
#include <Com\Decoration\DecoraBase.h>

namespace COM	{

class COM_EXPORT CFocusCross : public CDecoraBase
{
public:
	CFocusCross();
	~CFocusCross();

	bool Init(int width, int height, const osg::Vec4& color, osg::Group* parent);

	bool Show(bool bShow=true);

private:
	osg::ref_ptr<osg::Switch>	m_cross;
};

}