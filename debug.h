/*
 * debug.h -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
#define DEBUG_TILECOORDS        0x00000080
#define DEBUG_PATHS             0x00000100
#define DEBUG_WORLDWINDOW       0x00000200
#define DEBUG_DATABASE          0x00000400

#define ENABLE_DEBUGGING

#ifdef ENABLE_DEBUGGING
# define fDebug(FLAG) if(debugflags & (FLAG)) qDebug()
# define fEnabled(FLAG) (debugflags & (FLAG))
#else
# define fDebug(FLAG) if(false) qDebug()
# define fEnabled(FLAG) (false)
#endif

#endif // DEBUG_H
