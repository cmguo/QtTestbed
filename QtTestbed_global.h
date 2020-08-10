#ifndef QTTESTBED_GLOBAL_H
#define QTTESTBED_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTTESTBED_LIBRARY)
#  define QTTESTBED_EXPORT Q_DECL_EXPORT
#else
#  define QTTESTBED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTTESTBED_GLOBAL_H
