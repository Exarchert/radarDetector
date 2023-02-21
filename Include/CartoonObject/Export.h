#  if defined( TDEM_LIBRARY_STATIC )
#    define CARTOONOBJECT_EXPORT
#  elif defined( CARTOONOBJECT_LIBRARY )
#    define CARTOONOBJECT_EXPORT   __declspec(dllexport)
#  else
#    define CARTOONOBJECT_EXPORT   __declspec(dllimport)
#endif	