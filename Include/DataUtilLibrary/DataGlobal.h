#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(DATAUTILLIBRARY_LIB)
#  define DATAUTILLIBRARY_EXPORT Q_DECL_EXPORT
# else
#  define DATAUTILLIBRARY_EXPORT Q_DECL_IMPORT
# endif
#else
# define DATAUTILLIBRARY_EXPORT
#endif
