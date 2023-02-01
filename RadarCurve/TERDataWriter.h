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
	FILE *m_lpTERDataFile;//数据文件
	std::string m_strTimeTab;//时间戳

	std::string m_strPath;
	std::string m_strProjectName;
	std::string m_strProjectTab;
	
	int m_nSampleNum;//采样点数
	float m_fSampleRate;//采样率
	double m_dTimeWnd;//时窗
	float m_fDielectric;//介电常数
	double m_dPrecValue;//测量轮实际精度

	int m_nDataCount;//写入的数据总量
	double m_dTotalLen;//写入数据的距离长度

	bool m_bIsDataOne;
	std::vector<float> m_vecfDataOne;//返回的两段数据中的第一段
	std::vector<float> m_vecfDataTwo;//返回的两段数据中的第二段
};
