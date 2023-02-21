#ifndef __APPLICATION_GROCX_CONFIGURE_SET_H_FILE__
#define __APPLICATION_GROCX_CONFIGURE_SET_H_FILE__
#include <RadarCurve/export.h>
#include <string>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <map>
#include <vector>

class ConfigureSet : public osg::Referenced
{
    public:
        ConfigureSet();

		// 设置一个值
        void set( std::string const& group, std::string const& key, std::string const& value );
        std::string get( std::string const& group, std::string const& key );
		bool getGroupItemsName(std::string const& group, std::vector<std::string>& groupItemsName);

        void write();
        void read();
		void read(std::string const& cfgFile);

        void setFile( std::string const& fileName );

		std::string getGroupXML( std::string const& groupName );
		void fromGroupXML( std::string const& xmlStr );

    protected:
        typedef std::map< std::string, std::string > StringMap;
        typedef std::map< std::string, StringMap > ValueMap;

        ValueMap _cfgValue;
        std::string m_cfgFilePath;
};

#endif // __APPLICATION_GROCX_CONFIGURE_SET_H_FILE__
