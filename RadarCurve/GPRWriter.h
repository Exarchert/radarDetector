#pragma once
#include <RadarCurve/RadarData.h>
#include <string>
#include <fstream>
#include <RadarCurve/ConfigureSet.h>
#include "GPRHeader.h"
#include "Vector3D.h"

class GPRWriter
{
public:
	GPRWriter(void);
	~GPRWriter(void);

	bool openGPR( std::string const& filePath, std::string const &paramXML, int trackNo );
	bool closeGPR();

	void writeHead( int trackNum,std::string _projectNameTime );
	void writeRadarData( RadarData *lpData );

	void buchongHead();

protected:
	double getTotalLen( int iTracNo, int wheelCount );
protected:
	FILE *_lpFile;

	ConfigureSet _set;

	double _dTimeWnd;
	int _samNum;

	int _wheelNum[8];


	double _precValue;
	float _sampRate;

	int _markNo;

	double _totalLen;

	int _iTrackNo;


	_GPR_TRACE _gprTrace;

	_GPR_HEAD sGPRHead;

	int m_iTrcNum;


		
	CVector3D cFirstVector;

	long iFirstTrcOffset;


};
