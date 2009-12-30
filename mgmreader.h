#ifndef MGMREADER_H
#define MGMREADER_H

#include <QString>

struct mgm_file
{
    unsigned short int no_tiles;
};

class MGMReader
{
    public:
        MGMReader();
        static void read(const QString& filename);
};

#endif // MGMREADER_H
