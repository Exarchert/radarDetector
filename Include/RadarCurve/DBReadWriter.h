#pragma once
#include <RadarCurve/export.h>


class RADAR_EXPORT DBReadWriter
{
public:
	DBReadWriter(void);
	~DBReadWriter(void);

	void Write( void *buff, unsigned int len );
	void Read( void *buff, unsigned int len );

	void ReadCfg();
	void WriteCfg();

};
