#include "stdafx.h"
#include <RadarCurve/ConfigureSet.h>
#include <tinyxml.h>
#include <string>
#include <sstream>
#include <Com/KernelContext/KernelContext.h>
#include <Kernel/KernelExport.h>

ConfigureSet::ConfigureSet()
{ 

}


void ConfigureSet::set( std::string const& group, std::string const&key, std::string const& value )
{
    _cfgValue[group][key] = value;
}

std::string ConfigureSet::get( std::string const& group, std::string const& key )
{
    if ( _cfgValue.find( group ) == _cfgValue.end() )
    {
        return "";
    }

    if ( _cfgValue[group].find ( key ) == _cfgValue[group].end() )
    {
        return "";
    }

    return _cfgValue[group][key];
}

bool ConfigureSet::getGroupItemsName(std::string const& group, std::vector<std::string>& groupItemsName)
{
	if ( _cfgValue.find( group ) == _cfgValue.end() )
	{
		return false;
	}

	StringMap pStringMap =  _cfgValue[group];

	StringMap::iterator itor = pStringMap.begin();
	for (itor; itor != pStringMap.end();++itor)
	{
		groupItemsName.push_back(itor->first);
	}

	return true;
}

void ConfigureSet::write()
{
	COM::CKernelContext *lpKC = Kernel::GetpKC();
	if ( !lpKC )
	{

		return;
	}
	USES_CONVERSION;
	//std::string cfgFile = std::string(W2A(lpKC->m_BinPath)) + "\\" + m_cfgFilePath;
    TiXmlDocument doc;

    TiXmlElement *rootElement = new TiXmlElement("cfg");
    doc.LinkEndChild(rootElement);

    for (ValueMap::iterator it = _cfgValue.begin(); it != _cfgValue.end(); it++)
    {
        std::string group = (*it).first;
        StringMap &sm = (*it).second;
        TiXmlElement *groupElement = new TiXmlElement("group");

        groupElement->SetAttribute("name", group.c_str());
        rootElement->LinkEndChild(groupElement);
        for (StringMap::iterator sit = sm.begin(); sit != sm.end(); sit++)
        {
            std::string key = (*sit).first;
            std::string value = (*sit).second;
            TiXmlElement *valueElement = new TiXmlElement("item");

            valueElement->SetAttribute("name", key.c_str());
            valueElement->SetAttribute("value", value.c_str());
            groupElement->LinkEndChild(valueElement);
        }
    }

    doc.SaveFile(m_cfgFilePath.c_str());
}

void ConfigureSet::read(std::string const& cfgFile)
{
	COM::CKernelContext *lpKC = Kernel::GetpKC();
	if ( !lpKC )
	{

		return;
	}

	m_cfgFilePath = cfgFile;
	USES_CONVERSION;
	//std::string fullCfgFile = std::string(W2A(lpKC->m_BinPath)) + "\\" + cfgFile;
	TiXmlDocument doc;
	if ( !doc.LoadFile(cfgFile.c_str()) )
	{
		::MessageBox(NULL, A2W(cfgFile.c_str()),L"¼ÓÔØ´íÎó", MB_OK);
		return;
	}

	TiXmlElement *lpRootElement = doc.RootElement();
	if (!lpRootElement)
	{
		return;
	}

	TiXmlElement *lpGroupElement = lpRootElement->FirstChildElement("group");
	while (lpGroupElement)
	{
		if (!lpGroupElement->Attribute("name"))
		{
			lpGroupElement = lpGroupElement->NextSiblingElement("group");
			continue;
		}
		StringMap &sm = _cfgValue[lpGroupElement->Attribute("name")];
		TiXmlElement *valueElement = lpGroupElement->FirstChildElement("item");

		while (valueElement)
		{
			const char *key = valueElement->Attribute("name");
			const char *value = valueElement->Attribute("value");
			if (key && value)
			{
				sm[key] = value;
			}
			else
			{
				printf("err, name or value is emprty\n");
			}

			valueElement = valueElement->NextSiblingElement("item");
		}
		lpGroupElement = lpGroupElement->NextSiblingElement("group");
	}
}

void ConfigureSet::read()
{
	read(m_cfgFilePath);
}
void ConfigureSet::setFile( std::string const& fileName )
{
    m_cfgFilePath = fileName;
}


std::string ConfigureSet::getGroupXML( std::string const& groupName )
{
	TiXmlDocument doc;

	TiXmlElement *rootElement = new TiXmlElement("cfg");
	doc.LinkEndChild(rootElement);

	ValueMap::iterator it = _cfgValue.find( groupName );
	if ( it != _cfgValue.end() )
	{
		std::string group = (*it).first;
		StringMap &sm = (*it).second;
		TiXmlElement *groupElement = new TiXmlElement("group");

		groupElement->SetAttribute("name", group.c_str());
		rootElement->LinkEndChild(groupElement);
		for (StringMap::iterator sit = sm.begin(); sit != sm.end(); sit++)
		{
			std::string key = (*sit).first;
			std::string value = (*sit).second;
			TiXmlElement *valueElement = new TiXmlElement("item");

			valueElement->SetAttribute("name", key.c_str());
			valueElement->SetAttribute("value", value.c_str());
			groupElement->LinkEndChild(valueElement);
		}
	}

	std::string result;
	TiXmlPrinter printer;
	doc.Accept( &printer );
	if ( printer.CStr() )
	{
		result = printer.CStr();
	}
	return result;
}

void ConfigureSet::fromGroupXML( std::string const& xmlStr )
{
	TiXmlDocument doc;
	if ( !doc.Parse( xmlStr.c_str() ) )
	{
		//	return;
	}

	TiXmlElement *lpRootElement = doc.RootElement();
	if (!lpRootElement)
	{
		return;
	}

	TiXmlElement *lpGroupElement = lpRootElement->FirstChildElement("group");
	while (lpGroupElement)
	{
		if (!lpGroupElement->Attribute("name"))
		{
			lpGroupElement = lpGroupElement->NextSiblingElement("group");
			continue;
		}
		StringMap &sm = _cfgValue[lpGroupElement->Attribute("name")];
		TiXmlElement *valueElement = lpGroupElement->FirstChildElement("item");

		while (valueElement)
		{
			const char *key = valueElement->Attribute("name");
			const char *value = valueElement->Attribute("value");
			if (key && value)
			{
				sm[key] = value;
			}
			else
			{
				printf("err, name or value is emprty\n");
			}

			valueElement = valueElement->NextSiblingElement("item");
		}
		lpGroupElement = lpGroupElement->NextSiblingElement("group");
	}
}