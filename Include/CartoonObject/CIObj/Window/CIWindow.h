#pragma once
#include <CartoonObject\Export.h>
#include <COM\Cartoon\CartoonItem.h>
#include <Object\ShowObj\screenwindow\SOWindow.h>

namespace CartoonObject
{

class CARTOONOBJECT_EXPORT CCIWindow : public COM::CCartoonItem,  public EMObj::CSOWindow
{
public:
	CCIWindow(void);
	~CCIWindow(void);
};

}
