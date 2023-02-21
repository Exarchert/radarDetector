#ifndef PIPLEMANAGE_EXPORT_H
#define PIPLEMANAGE_EXPORT_H

#if defined(_MSC_VER) && defined(OSG_DISABLE_MSVC_WARNINGS)
#pragma warning( disable : 4786 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#if defined(OSG_LIBRARY_STATIC)
#   define PipleManager_EXPORT
#elif defined(PIPLEMANAGER_LIBRARY)
#   define PipleManager_EXPORT __declspec(dllexport)  
#else
#   define PipleManager_EXPORT __declspec(dllimport)  
#endif
#else
#define OSGCG_EXPORT
#endif

#endif