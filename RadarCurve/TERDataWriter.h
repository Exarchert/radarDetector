#pragma once
#include <RadarCurve/TERData.h>
#include <string>
#include <fstream>
#include <RadarCurve/ConfigureSet.h>

class TERDataWriter
{
public:
	TERDataWriter(void);
	~TERDataWriter(void);

	bool openUep( std::string const& filePath );
	void writeUepHead();
	void writeUepData( TERData *lpData );
	void copyUepData();
	void writeUepTail();
	bool closeUep();

protected:
	ConfigureSet m_cfgset;//cfg
	FILE *m_lpTERDataFile;//�����ļ�
	std::string m_strTimeTab;//ʱ���

	std::string m_strPath;
	std::string m_strProjectName;
	std::string m_strProjectTab;
	
	int m_nSampleNum;//��������
	float m_fSampleRate;//������
	double m_dTimeWnd;//ʱ��
	float m_fDielectric;//��糣��
	double m_dPrecValue;//������ʵ�ʾ���

	int m_nDataCount;//д�����������
	double m_dTotalLen;//д�����ݵľ��볤��

	bool m_bIsDataOne;
	std::vector<float> m_vecfDataOne;//���ص����������еĵ�һ��
	std::vector<float> m_vecfDataTwo;//���ص����������еĵڶ���
};
