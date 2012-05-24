#ifndef DEBUG_H
#define DEBUG_H

#include <QtCore>

extern quint64 debugflags;

#define DEBUG_NETWORK   0x00000001
#define DEBUG_DISK      0x00000002

#define ENABLE_DEBUGGING

#ifdef ENABLE_DEBUGGING
# define debugBlock(FLAG) if(debugflags & (FLAG))
#else
# define debugBlock(FLAG) if(false)
#endif

#endif // DEBUG_H
