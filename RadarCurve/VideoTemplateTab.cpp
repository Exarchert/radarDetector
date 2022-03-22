#include "StdAfx.h"
#include "VideoTemplateTab.h"
#include <sstream>
#include <osg/io_utils>
#include <osg/Vec3d>
#include <vector>
#include "RadarCurve/ConfigureSet.h"
#include "RadarCurve/RadarManager.h"
VideoTemplateTab::VideoTemplateTab()
{
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();
	std::string strDBUserName = pConfigureSet->get("db", "user").c_str();
	_strDBUserName = strDBUserName + ".";
}

std::string VideoTemplateTab::getSelectSql()
{
	
	return "select ID, RECVTIME, filePath, CameraNum from " + _strDBUserName + _projectName;
}

void VideoTemplateTab::insert( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{
	const VideoRow *lpNameRow = dynamic_cast<const VideoRow*>(lpRow);
	if ( !lpNameRow )
	{
		return;
	}

	std::string openSql = "insert into " + _strDBUserName + _projectName + " (RECVTIME, FILEPATH, CAMERANUM) values(:f1<int>,:f2<char[255]>,:f3<int>)";

	if ( !prepareStream( lpHelp, openSql, true ) )
	{
		return;
	}

	int t = lpNameRow->_recvTime;
	_lpRecordSet->StreamIn( t );
	std::string filePath = lpNameRow->_filePath;
	_lpRecordSet->StreamIn( filePath );

	t = lpNameRow->_cameraNum;
	_lpRecordSet->StreamIn( t );


	clearStream();
}

void VideoTemplateTab::update( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{

}
void VideoTemplateTab::del( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{

}

DBRow *VideoTemplateTab::readImpl( DBAPI::CRecordSetBase *lpRecordSet )
{
	VideoRow *lpRow = new VideoRow;

	int value;
	lpRecordSet->GetFieldValue( long( 0 ), value );
	lpRow->setID( value );


	lpRecordSet->GetFieldValue( long(1), value );
	lpRow->_recvTime = value;


	std::string filePath;
	lpRecordSet->GetFieldValue( long(2), filePath );
	lpRow->_filePath = filePath;

	lpRecordSet->GetFieldValue( long( 3 ), value );
	lpRow->_cameraNum = value;
	return lpRow;
}
bool VideoTemplateTab::createTab(CDBConnectHelp *lpHelp)
{
	std::string seqSql = "create sequence " + _strDBUserName + _projectName +
						"_seq "
						"minvalue 1 "
						"maxvalue 99999999999 "
						"start with 1 "
						"increment by 1 "
						"cache 20"
						;
	std::string createSql = "create table " + _strDBUserName + _projectName
		 + "( "
		 + "id       INTEGER not null,"
		 + "recvtime INTEGER,"
		 + "filepath varchar(256), "
		 + "cameranum INTEGER "
		 + ")"
		 + "tablespace RADAR "
		 + "pctfree 10 "
		 + "initrans 1 "
		 + "maxtrans 255 "
		 + "storage "
		 + "("
		 + "initial 10M "
		 + "next 10M "
		 + "minextents 1 "
		 + "maxextents unlimited "
		 + ") ";
	std::string createSuoyin =
		 + "create index " + _strDBUserName + _projectName + "_SUOYIN on " + _strDBUserName + _projectName + " (RECVTIME) "
		 + "tablespace RADAR "
		 + "pctfree 10 "
		 + "initrans 2 "
		 + "maxtrans 255 "
		 + "storage "
		 + "("
		 + "initial 64K "
		 + "next 1M "
		 + "minextents 1 "
		 + "maxextents unlimited "
		 + ")";
	std::string createIndex =
		 + "alter table " + _strDBUserName + _projectName 
		 + " add constraint " + _strDBUserName + _projectName + "_KEY primary key (ID) "
		 + " using index  "
		 + " tablespace RADAR "
		 + " pctfree 10 "
		 + " initrans 2 "
		 + " maxtrans 255 "
		 + " storage "
		 + "( "
		 + " initial 64K "
		 + " next 1M "
		 + " minextents 1 "
		 + " maxextents unlimited "
		 + ")"
		;
	std::string createCameraNumSuoyin =
		+ "create index " + _strDBUserName + _projectName + "_CN_SY on " + _strDBUserName + _projectName + " (CameraNum) "
		+ "tablespace RADAR "
		+ "pctfree 10 "
		+ "initrans 2 "
		+ "maxtrans 255 "
		+ "storage "
		+ "("
		+ "initial 64K "
		+ "next 1M "
		+ "minextents 1 "
		+ "maxextents unlimited "
		+ ")";

		std::string createTigger = "CREATE OR REPLACE TRIGGER " + _strDBUserName + _projectName+ "_trigger BEFORE INSERT ON " + _strDBUserName + _projectName  + " FOR EACH ROW "
									+ "BEGIN SELECT " +_strDBUserName + _projectName + "_seq.NEXTVAL INTO :NEW.ID FROM DUAL; END;";

		lpHelp->ExecSql( createSql.c_str() );
		lpHelp->ExecSql( createSuoyin.c_str() );
		lpHelp->ExecSql( createIndex.c_str() );
		lpHelp->ExecSql( seqSql.c_str() );
		lpHelp->ExecSql( createTigger.c_str() );
	return true;
}

void VideoTemplateTab::setProjectName( std::string const& projectName )
{
	_projectName = "video_" + projectName + "_tab";
}
void VideoTemplateTab::drop(CDBConnectHelp *lpHelp)
{
	std::string dropSql = "drop table " + _strDBUserName + _projectName;
	lpHelp->ExecSql( dropSql.c_str() );
}

std::string& VideoTemplateTab::getProjectName()
{
	return _projectName;
}

bool VideoTemplateTab::exportSqlDataVideoIni(std::string strFileExportPath)
{
	if ( !_lpRecordSet )
	{
		return false;
	}
	FILE * fp0=fopen(strFileExportPath.c_str(),"wb");
	if (NULL == fp0)
	{
		return false;
	}
	fclose(fp0);
	fp0=fopen(strFileExportPath.c_str(),"ab+");
	std::stringstream ss;

	int inde = 0;
	int value = 0;;
	while( !_lpRecordSet->IsEOF())
	{
		if (inde != 0)
		{
			ss << "\n";
		}
		else
		{
			inde = 1;
		}
		_lpRecordSet->GetFieldValue( long(1), value );
		ss << value;
		ss << "|";
		std::string filePath;
		_lpRecordSet->GetFieldValue( long(2), filePath );
		ss << filePath;
		ss << "|";
		_lpRecordSet->GetFieldValue( long( 3 ), value );
		ss << value;
		ss << "|";
		_lpRecordSet->MoveNext();
	}
	fwrite( ss.str().c_str(), ss.str().size(), 1, fp0);
	fclose(fp0);
	return true;
}

bool VideoTemplateTab::getPicPath(CDBConnectHelp *lpHelp, std::string strReceiveTime,std::string& strPicPathA,std::string& strPicPathB)
{
	std::string selectSql = "select DISTINCT(ID), recvtime, FILEPATH, Cameranum from " + _projectName +" where RecvTime ="+strReceiveTime + " and Cameranum=1";
	if ( !open( lpHelp, selectSql ) )
	{
		return 0;
	}

 	RowVector rv = read( 1 );
 	if ( rv.size() == 0 )
 	{
 		close();
 		return 0;
 	}
 
 	close();
 
 	VideoRow *lpVR = dynamic_cast<VideoRow*>(rv[0].get());
 	if ( !lpVR )
 	{
 		return 0;
 	}

	strPicPathA = lpVR->_filePath;

	selectSql = "select DISTINCT(ID), recvtime, FILEPATH, Cameranum from " + _projectName +" where RecvTime ="+strReceiveTime +" and Cameranum=2";
	if ( !open( lpHelp, selectSql ) )
	{
		return 0;
	}

	rv = read( 1 );
	if ( rv.size() == 0 )
	{
		close();
		return 0;
	}

	close();

	lpVR = dynamic_cast<VideoRow*>(rv[0].get());
	if ( !lpVR )
	{
		return 0;
	}
	strPicPathB = lpVR->_filePath;
	return true;
}