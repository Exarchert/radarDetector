#pragma once
#include <osg/Referenced>

class OracleWrapper : public osg::Referenced
{
public:
	OracleWrapper(void);
	~OracleWrapper(void);

	bool open();
	bool close();

	void readData();
	void writeData();
};
