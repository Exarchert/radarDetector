#include "StdAfx.h"

//C++
#include <sstream>
#include <vector>

#include <COM\log\com_log.h>
#include <osg/io_utils>

#include "RadarCurve\ConfigureSet.h"
#include <RadarCurve/RadarData.h>
#include "DataTemplateTab.h"
#include "DataCountTab.h"
#include "RadarCurve/RadarManager.h"

double _string_to_double(std::string s )
{
	double n = 0;
	int index = 0;
	int iLengthX = 0;
	int iLengthY = 0;
	index = s.find(".");
	if ( index <= 0)
	{
		return 0;
	}
	iLengthX = index;
	iLengthY = s.size() -  iLengthX - 1;
	std::string strRight;
	strRight = s.substr(index + 1);
	char * p = NULL;
	char aTemp;
	p =(char* )s.c_str();
	int iTemp;
	for (int i = index - 1 ; i > -1;--i)
	{
		aTemp = *p++;
		iTemp = aTemp - 48;
		n += iTemp *  pow(10.,i);
	}
	p =(char* )strRight.c_str();
	for (int i = 1 ; i <= iLengthY;++i)
	{
		aTemp = *p++;

		iTemp = aTemp - 48;
		n += iTemp *  pow(0.1,i);
	}
	return n;
}


double _char_to_double(char* a )
{
	std::string s = a;
	return _string_to_double(s);
}


std::vector<std::string> splittEx(const std::string& src, std::string separate_character)
{
	std::vector<std::string> strs;

	int separate_characterLen = separate_character.size();
	int lastPosition = 0,index = -1;
	while (-1 != (index = src.find(separate_character,lastPosition)))
	{
		strs.push_back(src.substr(lastPosition,index - lastPosition));
		lastPosition = index + separate_characterLen;
	}
	std::string lastString = src.substr(lastPosition);
	if (!lastString.empty())
		strs.push_back(lastString);
	return strs;
}

DataTemplateTab::DataTemplateTab(void)
{
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();
	std::string strDBUserName = pConfigureSet->get("db", "user").c_str();
	_strDBUserName = strDBUserName + ".";
}

DataTemplateTab::~DataTemplateTab(void)
{
}


bool DataTemplateTab::GetAllPoint(CDBConnectHelp *lpHelp,const std::string strProjectName,const std::string strIndex,std::vector< osg::Vec3f>& _vec)
//bool DataTemplateTab::GetAllPoint(CDBConnectHelp *lpHelp,const std::string strProjectName,const std::string strIndex,std::vector< osg::Vec3d>& _vec)
{
	if ((NULL == lpHelp) ||  (1 > strProjectName.size()) || (1 > strIndex.size()) )
	{
		return false;
	}
	setProjectName(strProjectName);
	std::string strSql = "select GpsValue from " ;
	strSql += _strDBUserName;
	strSql += _projectName;
	strSql += " where( CHANNELINDEX='" ;
	strSql += strIndex ;
	strSql += "') order by id";
	USES_CONVERSION;
	//AfxMessageBox( A2W(strSql.c_str()));
	//strSql = "select GpsValue from radar.data_1466073497_tab where( CHANNELINDEX='0') order by id";

	if (!open( lpHelp,strSql ))
	{
		return false;
	}
	osg::Vec3f posValue(0.0,0.0,0.0);
	osg::Vec3f posZero(0.0,0.0,0.0);
	while( !_lpRecordSet->IsEOF())
	{
		std::string strValue;
		osg::Vec3f posTemp(0.0,0.0,0.0);
		std::vector<std::string> _vStr;


		_lpRecordSet->GetFieldValue( long ( 0 ), strValue );
		_vStr = splittEx(strValue, " ");
		//double dou = 0;
		for (int i = 0; i < _vStr.size();++i )
		{
			//pos._v[i] = atof(_vStr[i].c_str());
			//dou = 0;
			//sscanf(_vStr[i].c_str(),"%9f",&dou);
			posTemp._v[i] = _string_to_double(_vStr[i]);
		}
		//std::stringstream ss;
		//ss << strValue;
		//ss >> pos;

		if (posZero !=  posTemp)
		{
			if (posZero == posValue)
			{
				posValue = posTemp;
				_vec.push_back(posTemp);
			}
			else
			{
				if (posTemp != posValue)
				{
					posValue = posTemp;
					_vec.push_back(posTemp);
				}
			}
		}
		_lpRecordSet->MoveNext();
		//AfxMessageBox( A2W(strValue.c_str()));
	}
	close();
	return true;
}



void DataTemplateTab::setProjectName( std::string const& projectName )
{
	_projectName = "data_" + projectName + "_tab";
}

std::string DataTemplateTab::getProjectName()const
{
	return _projectName;
}

std::string DataTemplateTab::getSelectSql()
{
	return "select ID, ReceiveTime,Sample,SampleRatio,GpsValue, Len, Mark, RadarData from " + _strDBUserName + _projectName;
}

std::string DataTemplateTab::getSelectSqlAll()
{                                                              
	return "select ID, ReceiveTime,Sample,SampleRatio,GpsValue,ChannelIndex, Len, Mark, RadarData from " + _strDBUserName + _projectName;
}

DBRow *DataTemplateTab::readImpl( DBAPI::CRecordSetBase *lpRecordSet )
{
	RadarData *lpRD = new RadarData;

	int value = 0;
	lpRecordSet->GetFieldValue( long(0), value);
	lpRD->setID( value );

	lpRecordSet->GetFieldValue( long(1), value);
	lpRD->setTime( value );

	lpRecordSet->GetFieldValue( long(2), value);
	lpRD->setSampleCount( value );

	float fv;
	lpRecordSet->GetFieldValue( long(3), fv);
	lpRD->setSampleRatio( fv );

	std::string strValue;
	lpRecordSet->GetFieldValue( long ( 4 ), strValue );
	osg::Vec3d pos;
	std::stringstream ss;
	ss << strValue;
	ss >> pos;
	lpRD->setPosition( pos );

	lpRecordSet->GetFieldValue( long(5), fv);
	lpRD->setLen( fv );

	bool bv;
	lpRecordSet->GetFieldValue( long ( 6 ), value );
	bv = value > 0;
	lpRD->setMark( bv );

	char *lpBuff = new char [lpRD->getSampleCount() * sizeof( short ) ];

	int len = lpRD->getSampleCount() * sizeof( short ) ;
	lpRecordSet->GetFieldValue( long ( 7 ), lpBuff, &len );

	lpRD->setData( (unsigned char *)lpBuff, len );

	delete lpBuff;
	return lpRD;
}

void DataTemplateTab::openStream(CDBConnectHelp *lpHelp)
{
	std::string openSql = "insert into " + _strDBUserName + _projectName + " (ID,ReceiveTime,Sample,SampleRatio,GpsValue, ChannelIndex,Len, Mark, RadarData)"
		" values(:f0<long>,:f1<int>,:f2<int>,:f3<float>,:f4<char[63]>,:f5<int>,:f6<float>,:f7<int>,empty_blob())"
		" returning RadarData into :f8<blob> ";


	try
	{
		if ( !prepareStream( lpHelp, openSql, true ) )
		{
			return;
		}
	}
	catch( ... )
	{

	}
}



void DataTemplateTab::insert( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{
	const RadarData *lpNameRow = dynamic_cast<const RadarData*>(lpRow);
	if ( !lpNameRow )
	{
		return;
	}
 	std::string openSql = "insert into " + _strDBUserName + _projectName + " (ReceiveTime,Sample,SampleRatio,GpsValue, ChannelIndex,Len, Mark, RadarData)"
 		" values(:f1<int>,:f2<int>,:f3<float>,:f4<char[63]>,:f5<int>,:f6<float>,:f7<int>,empty_blob())"
 		" returning RadarData into :f8<blob> ";
 
	if ( !prepareStream( lpHelp, openSql, true ) )
	{
		return;
	}

	long id = lpNameRow->getID();
	_lpRecordSet->StreamIn( id );

	int value = lpNameRow->getTime();
	_lpRecordSet->StreamIn( value );

	value = lpNameRow->getSampleCount();
	_lpRecordSet->StreamIn( value );

	float fv = lpNameRow->getSampleRatio();
	_lpRecordSet->StreamIn( fv );

	char posBuff[64];
	sprintf( posBuff, "%0.7f %0.7f %0.7f", lpNameRow->getPosition().x(), lpNameRow->getPosition().y(), lpNameRow->getPosition().z() );
	// 		std::stringstream ss;
	// 		ss << posBuff;//lpNameRow->getPosition();
	_lpRecordSet->StreamIn( std::string(posBuff) );

	int channelIndex = lpNameRow->getChannelIndex();
	_lpRecordSet->StreamIn( channelIndex );

	fv = lpNameRow->getLen();
	_lpRecordSet->StreamIn( fv );

	int imark = lpNameRow->getMark();
	_lpRecordSet->StreamIn( imark );

	int len = 0;
	int dataSize = lpNameRow->getSampleCount() * sizeof ( short );
	_lpRecordSet->StreamIn( (char*)lpNameRow->getData( len ), dataSize );//
	clearStream();

}


void DataTemplateTab::insertEX( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{
	const RadarData *lpNameRow = dynamic_cast<const RadarData*>(lpRow);
	if ( !lpNameRow )
	{
		return;
	}

// 	std::string openSql = "insert into " + _projectName + " (ReceiveTime,Sample,SampleRatio,GpsValue, ChannelIndex,Len, Mark, RadarData)"
// 		" values(:f1<int>,:f2<int>,:f3<float>,:f4<char[63]>,:f5<int>,:f6<float>,:f7<int>,empty_blob())"
// 		" returning RadarData into :f8<blob> ";
// 
	try
	{
//// 		if ( !prepareStream( lpHelp, openSql, true ) )
//// 		{
//// 			return;
//// 		}

		//---------------------------------------------------------
		long id = lpNameRow->getID();
		_lpRecordSet->StreamIn( id );

		int value = lpNameRow->getTime();
		_lpRecordSet->StreamIn( value );

		value = lpNameRow->getSampleCount();
		_lpRecordSet->StreamIn( value );

		float fv = lpNameRow->getSampleRatio();
		_lpRecordSet->StreamIn( fv );

		char posBuff[64];
		sprintf( posBuff, "%0.7f %0.7f %0.7f", lpNameRow->getPosition().x(), lpNameRow->getPosition().y(), lpNameRow->getPosition().z() );
// 		std::stringstream ss;
// 		ss << posBuff;//lpNameRow->getPosition();
		_lpRecordSet->StreamIn( std::string(posBuff) );

		int channelIndex = lpNameRow->getChannelIndex();
		_lpRecordSet->StreamIn( channelIndex );

		fv = lpNameRow->getLen();
		_lpRecordSet->StreamIn( fv );

		int imark = lpNameRow->getMark();
		_lpRecordSet->StreamIn( imark );


		int len = 0;
		int dataSize = lpNameRow->getSampleCount() * sizeof ( short );
		_lpRecordSet->StreamIn( (char*)lpNameRow->getData( len ), dataSize );
	}
	
	catch (...)
	{
		AfxMessageBox(L"存数据库：插入雷达数据报错！");
	}

}

void DataTemplateTab::closeStream()
{
	clearStream();
}

void DataTemplateTab::update( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{
	// 不提供更新功能
	return;
}
void DataTemplateTab::del( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{
	// 不提供删除功能
	return;
}
bool DataTemplateTab::createTab(CDBConnectHelp *lpHelp)
{
	std::string seqSql = "create sequence " + _strDBUserName + _projectName +
		"_seq "
		"minvalue 1 "
		"maxvalue 99999999999 "
		"start with 1 "
		"increment by 1 "
		"cache 20"
		;
	std::string createSql = "create table " + _strDBUserName + _projectName + 
		"( "
		"id           NUMBER not null, "
		"receivetime  INTEGER, "
		"sample       INTEGER, "
		"sampleratio  FLOAT, "
		"gpsvalue     VARCHAR2(64), "
		"channelindex INTEGER, "
		"len		  FLOAT, "
		"Mark		  INTEGER, "
		"radardata    BLOB "
		")"
		"tablespace RADAR "
		"pctfree 10 "
		"initrans 1 "
		"maxtrans 255 "
		"storage "
		"( "
		"initial 10M "
		"next 10M "
		"minextents 1 "
		"maxextents unlimited "
		")";
	std::string createRecvTimeSuoYin = "create index " + _strDBUserName + _projectName + "_CT_SUOYIN on " + _strDBUserName + _projectName + " (RECEIVETIME)"
		"tablespace RADAR "
		"pctfree 10 "
		"initrans 2 "
		"maxtrans 255 "
		"storage "
		"( "
		"initial 64K "
		"next 1M "
		"minextents 1 "
		"maxextents unlimited "
		")";
	std::string createChannelIndex = "create index " + _strDBUserName + _projectName  + "_CL_INDEX on " + _strDBUserName + _projectName + " (CHANNELINDEX) "
		"tablespace RADAR "
		"pctfree 10 "
		"initrans 2 "
		"maxtrans 255 "
		"storage "
		"( "
		"initial 64K "
		"next 1M "
		"minextents 1 "
		"maxextents unlimited "
		")";

	std::string createLenIndex = "create index " + _strDBUserName + _projectName  + "_LEN_IX on " + _strDBUserName + _projectName + " (LEN) "
		"tablespace RADAR "
		"pctfree 10 "
		"initrans 2 "
		"maxtrans 255 "
		"storage "
		"( "
		"initial 64K "
		"next 1M "
		"minextents 1 "
		"maxextents unlimited "
		")";

	std::string createIDIndex = "alter table " + _strDBUserName + _projectName + " add constraint " + _strDBUserName + _projectName + "_ID_KEY primary key (ID)"
		"using index "
		"tablespace RADAR "
		"pctfree 10 "
		"initrans 2 "
		"maxtrans 255 "
		"storage "
		"( "
		"initial 64K "
		"next 1M "
		"minextents 1 "
		"maxextents unlimited "
		")";

	std::string createTigger = "CREATE OR REPLACE TRIGGER " + _strDBUserName + _projectName+ "_trigger BEFORE INSERT ON " + _strDBUserName + _projectName  + " FOR EACH ROW "
		+ "BEGIN SELECT " + _strDBUserName + _projectName + "_seq.NEXTVAL INTO :NEW.ID FROM DUAL; END;";

	lpHelp->ExecSql( createSql.c_str() );

 	lpHelp->ExecSql( createIDIndex.c_str() );
	return true;
}
void DataTemplateTab::drop(CDBConnectHelp *lpHelp)
{
	std::string dropSql = "drop table " + _strDBUserName + _projectName;
	lpHelp->ExecSql( dropSql.c_str() );
}


bool DataTemplateTab::exportSqlDataRadarIni(std::string strFileNamePath,int iCount,int sampleCount )
{
	if ( !_lpRecordSet && iCount >0)
	{
		return false;
	}

	int iIndex = 0;
	FILE * fp0=fopen(strFileNamePath.c_str(),"wb");
	if (NULL == fp0)
	{
		return false;
	}
	RadarIOS stuData;
	int value = 0;
	char cBuf[64];
	unsigned char* cPTemp;
	float fv;
	cPTemp = (unsigned char*)stuData.ucBuf;
	int isampleCount = sampleCount * sizeof( short );
	std::string strValue;
	int iLen = 0;

	while( !_lpRecordSet->IsEOF() && iIndex < iCount)
	{
		std::stringstream ss;
		if (iIndex != 0)
		{
			//stuData.strBuf = "\n";
			ss << "\n";
		}
		//1创建时间
		_lpRecordSet->GetFieldValue( long(1), value);
		//itoa(value,cBuf,10);
		//stuData.strBuf += cBuf;
		//stuData.strBuf += "|";

		ss << value << "|";
		//2
		_lpRecordSet->GetFieldValue( long(2), value);
		//itoa(value,cBuf,10);
		//stuData.strBuf += cBuf;
		//stuData.strBuf += "|";

		ss << value << "|";
		_lpRecordSet->GetFieldValue( long(3), fv);
		//3
		//sprintf(cBuf,"%f",fv);
		//stuData.strBuf += cBuf;
		stuData.strBuf += "|";

		ss << fv << "|";
		//4怎么存就怎么取
		_lpRecordSet->GetFieldValue( long ( 4 ), strValue );
		//stuData.strBuf += strValue;
		//stuData.strBuf += "|";

		ss << strValue << "|";
		//5
		_lpRecordSet->GetFieldValue( long ( 5 ), value );
		//stuData.strBuf += itoa(value,cBuf,10);
		//stuData.strBuf += "|";

		ss << value << "|";
		_lpRecordSet->GetFieldValue( long(6), fv);
		////6
		//sprintf(cBuf,"%f",fv);
		//stuData.strBuf += cBuf;
		//stuData.strBuf += "|";

		ss << fv << "|";
		_lpRecordSet->GetFieldValue( long ( 7 ), value );
		//7
		if (0 < value )
		{
			//stuData.strBuf += "1|";
			ss << "1|";
		}
		else
		{
			//stuData.strBuf += "0|";
			ss << "0|";
		}
		//返回存的个数
		//channel = ss.str();

		value = ss.str().size();
		fwrite( ss.str().c_str(), ss.str().size(), 1, fp0);
		//fwrite( stuData.strBuf.c_str(), stuData.strBuf.size(), 1, fp0);
		//8
		_lpRecordSet->GetFieldValue( long ( 8 ), (char*)cPTemp, &isampleCount );
		//
		fwrite( cPTemp, isampleCount, 1, fp0);
		++iIndex;
		_lpRecordSet->MoveNext();

	}
	fclose(fp0);
	return true;
}


void CreateNeiChun(CString cstrPath)
{

	// 创建文件对象
	HANDLE hFile = CreateFile(cstrPath, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("创建文件对象失败,错误代码:%drn", GetLastError());
		return;
	}
	// 创建文件映射对象
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMap == NULL)
	{
		TRACE("创建文件映射对象失败,错误代码:%drn", GetLastError());
		return;
	}
	// 得到系统分配粒度
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	DWORD dwGran = SysInfo.dwAllocationGranularity;
	// 得到文件尺寸
	DWORD dwFileSizeHigh;
	__int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
	qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
	// 关闭文件对象
	CloseHandle(hFile);
	// 偏移地址 
	__int64 qwFileOffset = 0;

	while (qwFileSize > 0)
	{
		// 块大小
		DWORD dwBlockBytes = 1000 * dwGran;
		if (qwFileSize < 1000 * dwGran)
			dwBlockBytes = (DWORD)qwFileSize;

		// 映射视图
		LPBYTE lpbMapAddress = (LPBYTE)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS, 
			(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF),
			dwBlockBytes);
		if (lpbMapAddress == NULL)
		{
			TRACE("映射文件映射失败,错误代码:%drn", GetLastError());
			return;
		}
		// 对映射的视图进行访问
		for(DWORD i = 0; i < dwBlockBytes; i++)
			BYTE temp = *(lpbMapAddress + i);//可强转为char* 注意之后i就要移动8位
		// 撤消文件映像
		UnmapViewOfFile(lpbMapAddress);
		// 修正参数
		qwFileOffset += dwBlockBytes;
		qwFileSize -= dwBlockBytes;
	}
	// 关闭文件映射对象句柄
	CloseHandle(hFileMap);
	AfxMessageBox(L"成功完成对文件的访问");


}



bool DataTemplateTab::importSqlDataRadarIni( CDBConnectHelp *lpHelp,std::string strFileNamePath,int sampleCount )
{

	TabMaxIDTab tab;
	std::stringstream ss;
	tab.setTabName( getProjectName() );
	ss << tab.getSelectSql();
	long maxID  = tab.getMaxID( lpHelp, ss.str());
	unsigned char buf4096[4096];//最大支持4096


	std::string openSql = "insert into " + _strDBUserName + _projectName + " (ID,ReceiveTime,Sample,SampleRatio,GpsValue, ChannelIndex,Len, Mark, RadarData)"
		" values(:f0<long>,:f1<int>,:f2<int>,:f3<float>,:f4<char[63]>,:f5<int>,:f6<float>,:f7<int>,empty_blob())"
		" returning RadarData into :f8<blob> ";
	if ( !prepareStream( lpHelp, openSql, true ) )
	{
		//fclose(finDa);
		return false;
	}

	unsigned char ucTemp;
	char cTemp;
	int iDuanshu_ = 0;
	std::string strTemp = "";
	std::string strs[8];//
	std::string strPos;

	int dataSize = sampleCount * sizeof ( short );
	int iTemp = 0;
	float fv = 0;
	int iLine = 0;

	USES_CONVERSION;
	CString cstrPath = A2W(strFileNamePath.c_str());
	// 创建文件对象
	HANDLE hFile = CreateFile(cstrPath, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("创建文件对象失败,错误代码:%drn", GetLastError());
		return false;
	}
	// 创建文件映射对象
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMap == NULL)
	{
		TRACE("创建文件映射对象失败,错误代码:%drn", GetLastError());
		return false;
	}
	// 得到系统分配粒度
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	DWORD dwGran = SysInfo.dwAllocationGranularity;
	// 得到文件尺寸
	DWORD dwFileSizeHigh;
	__int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
	qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
	// 关闭文件对象
	CloseHandle(hFile);
	// 偏移地址 
	__int64 qwFileOffset = 0;

	while (qwFileSize > 0)
	{
		// 块大小
		DWORD dwBlockBytes = 52000 * dwGran;
		if (qwFileSize > dwBlockBytes)
		{
			dwBlockBytes = (DWORD)qwFileSize;
		}
		dwBlockBytes = (DWORD)qwFileSize;//一次申请
		// 映射视图
		LPBYTE lpbMapAddress = (LPBYTE)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS, 
			(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF),
			dwBlockBytes);
		if (lpbMapAddress == NULL)
		{
			TRACE("映射文件映射失败,错误代码:%drn", GetLastError());
			COM::WRITE_TO_LOG_PTR->WriteToLog(COM::LEVEL_COMMON,"importSqlDataRadarIni","申请文件内存太大");	
			AfxMessageBox(L"文件太大，不能导入！建议单个文件不大于1G.");
			return false;
		}
		// 对映射的视图进行访问

		char * cTempXXX = (char*)lpbMapAddress;
		for(DWORD i = 0; i < dwBlockBytes; )
		{
			cTemp = *cTempXXX++;
			++i;
			if (cTemp != '|')
			{
				strs[iDuanshu_] += cTemp;
				continue;
			}
			else
			{
				iDuanshu_++;
				strs[iDuanshu_] = "";
			}
			if (7 == iDuanshu_)
			{


				try
				{
					++maxID;
					_lpRecordSet->StreamIn( maxID );
					iTemp = (int)atoi(strs[0].c_str() );
					_lpRecordSet->StreamIn( iTemp);
					iTemp = (int)atoi(strs[1].c_str() );
					_lpRecordSet->StreamIn( iTemp);
					fv = atof(strs[2].c_str() );
					_lpRecordSet->StreamIn( fv);
					//4
					_lpRecordSet->StreamIn( strs[3] );
					//5
					iTemp = (int)atoi(strs[4].c_str() );
					_lpRecordSet->StreamIn( iTemp);
					//6
					fv = atof(strs[5].c_str() );
					_lpRecordSet->StreamIn( fv);
					//7
					if (0 == strcmp("0",strs[6].c_str()))
					{
						iTemp = 0;
						_lpRecordSet->StreamIn(iTemp);
					}
					else
					{
						iTemp = 1;
						_lpRecordSet->StreamIn(iTemp);
					}
					//8
					_lpRecordSet->StreamIn( cTempXXX, dataSize );

					cTempXXX += dataSize;
					++cTempXXX;
					i += dataSize;
					++i;
					iDuanshu_ = 0;
					strs[iDuanshu_] = "";
				}
				catch (...)
				{

					
				}

			}
		}
			//BYTE temp = *(lpbMapAddress + i);//可强转为char* 注意之后i就要移动8位
		// 撤消文件映像
		UnmapViewOfFile(lpbMapAddress);
		// 修正参数
		qwFileOffset += dwBlockBytes;
		qwFileSize -= dwBlockBytes;
	}
	// 关闭文件映射对象句柄
	CloseHandle(hFileMap);
	//AfxMessageBox(L"成功完成对文件的访问");
	return true;
}

