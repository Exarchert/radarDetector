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
	CBitmap _bmp; //�ڴ��г�����ʱͼ���λͼ
	bool _init;
	CRect _s;
};

