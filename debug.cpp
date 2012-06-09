#include "debug.h"

quint64 debugflags = 0;
const char *flagLabels[] = {
    "Network Activity",
    "Disk Activity",
    "TileFetcher Queues",
    "TileFetcher Requests",
    "Tile Layer",
    "Memory Cache",
    "Pixmap Cache",
    "Tile Coordinates",
    "Paths",
    NULL
};
