#  if defined( TDEM_LIBRARY_STATIC )
#    define OBJECT_EXPORT
#  elif defined( OBJECT_LIBRARY )
#    define OBJECT_EXPORT   __declspec(dllexport)
#  else
#    define OBJECT_EXPORT   __declspec(dllimport)
#endif