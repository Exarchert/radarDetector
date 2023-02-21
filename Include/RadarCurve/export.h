#  if defined( TDEM_LIBRARY_STATIC )
#    define RADAR_EXPORT
#  elif defined( RADAR_CURVE_LIBRARY )
#    define RADAR_EXPORT   __declspec(dllexport)
#  else
#    define RADAR_EXPORT   __declspec(dllimport)
#endif
