#include "stdafx.h"
#include "BmpDC.h"
#include <assert.h>


CBmpDC::CBmpDC()
{
	_init = false;


}


CBmpDC::~CBmpDC()
{

}
void CBmpDC::destory()
{
	if (_init)
	{
		_dcMem.DeleteDC();    
		_bmp.DeleteObject(); 
		_init = false;
	}
}

void CBmpDC::init(CDC *lpDC, CRect &s)
{
	if (_init)
	{
		destory();
	}
	assert(lpDC);

	_dcMem.CreateCompatibleDC(lpDC); 

	_bmp.CreateCompatibleBitmap(lpDC, s.Width(), s.Height());
	CBitmap *pOldBit = _dcMem.SelectObject(&_bmp);

	_init = true;
	_s.left = 0;
	_s.top = 0;
	_s.right = s.Width();
	_s.bottom = s.Height();
}

CDC *CBmpDC::getDC()
{
	return &_dcMem;
}

bool CBmpDC::isInit()
{
	return _init;
}

void CBmpDC::drawRect(CBrush *lpBrush)
{
	_dcMem.FillRect(_s, lpBrush);
}