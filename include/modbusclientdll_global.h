#ifndef MODBUSCLIENTDLL_GLOBAL_H
#define MODBUSCLIENTDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MODBUSCLIENTDLL_LIBRARY)
#  define MODBUSCLIENTDLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODBUSCLIENTDLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MODBUSCLIENTDLL_GLOBAL_H