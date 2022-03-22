#pragma once
class CBmpDC
{
public:
	CBmpDC();
	~CBmpDC();

	void init(CDC *lpDC, CRect &s);
	void destory();

	void drawRect( CBrush *lpBrush );

	CDC *getDC();

	bool isInit();

protected:
	CDC _dcMem;
	CBitmap _bmp; //内存中承载临时图象的位图
	bool _init;
	CRect _s;
};

