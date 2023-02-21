/* -*-c++-*- OpenSceneGraph Cookbook
 * Chapter 8 Recipe 7
 * Author: Wang Rui <wangray84 at gmail dot com>
*/

#ifndef OCTREEBUILDER
#define OCTREEBUILDER

#include <osg/Geode>
#include <osg/LOD>

class OctreeBuilder
{
public:
    OctreeBuilder() : _maxChildNumber(16), _maxTreeDepth(32), _maxLevel(0) {}
    int getMaxLevel() const { return _maxLevel; }
    //设置多少个几何图形可以包含在一个叶节点。默认是16.0
    void setMaxChildNumber( int max ) { _maxChildNumber = max; }

    int getMaxChildNumber() const { return _maxChildNumber; }
    //设置最大数量的水平
    void setMaxTreeDepth( int max ) { _maxTreeDepth = max; }
    int getMaxTreeDepth() const { return _maxTreeDepth; }
    
    typedef std::pair<std::string, osg::BoundingBox> ElementInfo;
    osg::Group* build( int depth, const osg::BoundingBox& total,
                       std::vector<ElementInfo>& elements );

	osg::Group* build(int depth,const osg::BoundingBox& total,
		std::vector<osg::ref_ptr<osg::Node>>& nodes);
    
protected:
    osg::LOD* createNewLevel( int level, const osg::Vec3& center, float radius );
    osg::Node* createElement( const std::string& id, const osg::Vec3& center, float radius );
    osg::Geode* createBoxForDebug( const osg::Vec3& max, const osg::Vec3& min );
    
    int _maxChildNumber;
    int _maxTreeDepth;
    int _maxLevel;
};

#endif
