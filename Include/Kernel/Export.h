#  if defined( TDEM_LIBRARY_STATIC )
#    define KERNEL_EXPORT
#  elif defined( KERNEL_LIBRARY )
#    define KERNEL_EXPORT   __declspec(dllexport)
#  else
#    define KERNEL_EXPORT   __declspec(dllimport)
#endif