#  if defined( TDEM_LIBRARY_STATIC )
#    define COM_EXPORT
#  elif defined( COM_LIBRARY )
#    define COM_EXPORT   __declspec(dllexport)
#  else
#    define COM_EXPORT   __declspec(dllimport)
#endif