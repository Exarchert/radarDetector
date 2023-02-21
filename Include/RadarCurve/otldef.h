#ifndef __OTL_DEF_H_FILE__
#define __OTL_DEF_H_FILE__


//#define OTL_STREAM_READ_ITERATOR_ON

// #define OTL_UNICODE
// 
// #define OTL_UNICODE_CHAR_TYPE TCHAR
#ifdef USE_ODBC
	#define OTL_ODBC
#else
	#define OTL_ORA11G_R2
#endif

#define OTL_STREAM_READ_ITERATOR_ON
//
//#define OTL_STL
//#define OTL_STREAM_POOLING_ON
#include "RadarCurve/otlv4.h"


#endif //__OTL_DEF_H_FILE__