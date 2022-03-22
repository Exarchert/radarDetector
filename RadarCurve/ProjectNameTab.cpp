#include "StdAfx.h"
#include "ProjectNameTab.h"
#include <sstream>
#include "RadarCurve\ConfigureSet.h"
#include "RadarCurve\RadarManager.h"

ProjectNameTab::ProjectNameTab(void)
{
	ConfigureSet* _configureSet = RadarManager::Instance()->getConfigureSet();
	std::string strDBUserName = _configureSet->get("db", "user").c_str();
	_strDBUserName = strDBUserName + ".";
	_projectName = "PROJECTTAB";

// 	TRACE(_strDBUserName.c_str());
// 	TRACE("\r\n\r\n--------------------------------------------------------------\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
// 	TRACE(_strDBUserName.c_str());
}

ProjectNameTab::~ProjectNameTab(void)
{
}
std::string ProjectNameTab::getSelectSql()
{

	std::string str = "select * from " + _strDBUserName + "projecttab";
	return str;
}

DBRow *ProjectNameTab::readImpl( DBAPI::CRecordSetBase *lpRecordSet )
{
	ProjectNameRow *lpRow = new ProjectNameRow;
	int value;
	std::string strValue;
	lpRecordSet->GetFieldValue( long(0), strValue);
	lpRow->_projectName = strValue;

	lpRecordSet->GetFieldValue( long( 1 ), value );
	lpRow->_createTime = value;

	float fv;
	lpRecordSet->GetFieldValue( long( 2 ), fv );
	lpRow->_startLen = fv;

	lpRecordSet->GetFieldValue( long( 3 ), fv );
	lpRow->_curLen = fv;

	lpRecordSet->GetFieldValue( long( 4 ), strValue);
	lpRow->_paramXML = strValue;
	
	return lpRow;
}


void ProjectNameTab::insert( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{

	const ProjectNameRow *lpNameRow = dynamic_cast<const ProjectNameRow*>(lpRow);
	if ( !lpNameRow )
	{
		return;
	}


	std::string openSql = "insert into ";
	openSql += _strDBUserName;
	openSql += _projectName;
	openSql += " values(:f1<char[256]>,:f2<int>,:f3<float>,:f4<float>,:f5<char[1024]>)";

	if ( !prepareStream( lpHelp, openSql, false ) )
	{
		return;
	}

	_lpRecordSet->StreamIn( std::string ( lpNameRow->_projectName ) );

	int t = lpNameRow->_createTime;
	_lpRecordSet->StreamIn( t );

	float len = lpNameRow->_startLen;
	_lpRecordSet->StreamIn( len );

	len = lpNameRow->_curLen;
	_lpRecordSet->StreamIn( len );

	std::string strValue = lpNameRow->_paramXML;
	_lpRecordSet->StreamIn( strValue );

	clearStream();
}

void ProjectNameTab::update( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{
	const ProjectNameRow *lpNameRow = dynamic_cast<const ProjectNameRow*>(lpRow);
	if ( !lpNameRow )
	{
		return;
	}
	
	std::string openSql = "update ";
	openSql += _strDBUserName;
	openSql += _projectName;
	openSql += " set STARTLEN=:f1<float>,PROJECTNAME=:f2<char[255]>,CURLEN=:f3<float>,PROJECTPARAM=:f4<char[1023]> where PROJECTCREATETIME=:f5<int>";
	if ( !prepareStream( lpHelp, openSql, false ) )
	{
		return;
	}

	float startLen = lpNameRow->_startLen;
	_lpRecordSet->StreamIn( startLen );

	_lpRecordSet->StreamIn( std::string ( lpNameRow->_projectName ) );



	float curLen = lpNameRow->_curLen;
	_lpRecordSet->StreamIn( curLen );

	std::string strValue = lpNameRow->_paramXML;
	_lpRecordSet->StreamIn( strValue );


	int t = lpNameRow->_createTime;
	_lpRecordSet->StreamIn( t );

	clearStream();
}
void ProjectNameTab::del( CDBConnectHelp *lpHelp, const DBRow *lpRow )
{
	const ProjectNameRow *lpNameRow = dynamic_cast<const ProjectNameRow*>(lpRow);
	std::string deleteSql;
	std::stringstream ss;
	ss << " delete " << _strDBUserName.c_str() << _projectName.c_str() << " where PROJECTCREATETIME=" << lpNameRow->_createTime;
	lpHelp->GetConnect()->ExecSql( ss.str().c_str());
}

bool ProjectNameTab::createTab( CDBConnectHelp *lpHelp )
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
		"projectname  VARCHAR2(256), "
		"projectcreatetime       INTEGER, "
		"startlen  FLOAT, "
		"curlen     FLOAT, "
		"PROJECTPARAM VARCHAR2(1024) "
		")"
		"tablespace RADAR "
		"pctfree 10 "
		"initrans 1 "
		"maxtrans 255 "
		"storage "
		"( "
		"initial 64K "
		"next 10M "
		"minextents 1 "
		"maxextents unlimited "
		")";

	TRACE(createSql.c_str());

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
	return true;
}

std::string ProjectNameTab::getProjectName() const
{

	return _projectName;
}
