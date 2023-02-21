#  if defined( TDEM_LIBRARY_STATIC )
#    define DATA_EXPORT
#  elif defined( DATA_LIBRARY )
#    define DATA_EXPORT   __declspec(dllexport)
#  else
#    define DATA_EXPORT   __declspec(dllimport)
#endif