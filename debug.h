#ifndef DEBUG_H
#define DEBUG_H

#include <QtCore>
#include <QtDebug>

extern quint64 debugflags;
extern const char *flagLabels[];

#define DEBUG_NETWORK           0x00000001
#define DEBUG_DISK              0x00000002
#define DEBUG_FETCHQUEUES       0x00000004
#define DEBUG_FETCHREQUESTS     0x00000008
#define DEBUG_TILELAYER         0x00000010
#define DEBUG_MEMCACHE          0x00000020
#define DEBUG_PIXMAPCACHE       0x00000040

#define ENABLE_DEBUGGING

#ifdef ENABLE_DEBUGGING
# define fDebug(FLAG) if(debugflags & (FLAG)) qDebug()
# define if_enabled(FLAG) if(debugflags & (FLAG))
#else
# define fDebug(FLAG) if(false) qDebug()
# define if_enabled(FLAG) if(false)
#endif

#endif // DEBUG_H
