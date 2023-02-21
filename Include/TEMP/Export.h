#  if defined( TDEM_LIBRARY_STATIC )
#    define COM_EXPORT
#  elif defined( TEMP_LIBRARY )
#    define TEMP_EXPORT   __declspec(dllexport)
#  else
#    define TEMP_EXPORT   __declspec(dllimport)
#endif