#pragma once
#include <CartoonObject\Export.h>
#include <CartoonObject\Cart\CartItem.h>
#include <Object\ShowObj\Model\SOModel.h>

namespace CartoonObject
{

class CARTOONOBJECT_EXPORT CCIModel : public CCartItem,  public EMObj::CSOModel
{
public:
	CCIModel(void);
	~CCIModel(void);
};

}
