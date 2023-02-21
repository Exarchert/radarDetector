#ifndef __BLUE_MOON_UTIL_SINGLETON_H_FILE__
#define __BLUE_MOON_UTIL_SINGLETON_H_FILE__
#include <osg/Referenced>
#include <osg/ref_ptr>

template<class T>
class Singleton : public osg::Referenced
{
  protected:

    Singleton(){};
    ~Singleton(){};
  public:
    static T *Instance( bool destory = false )
    {
      static osg::ref_ptr<T> S_T;
      if ( !S_T && !destory )
      {
        S_T = new T;
      }
      if ( destory )
      {
        S_T = NULL;
      }

      return S_T.get();
    }

    static void Destory()
    {
      Instance( true );
    }

};


#endif //__BLUE_MOON_UTIL_SINGLETON_H_FILE__
