#include <QtDebug>
#include <QtEndian>
#include <QFile>

#include "mgmreader.h"

MGMReader::MGMReader()
{
}

void MGMReader::read(const QString& filename)
{
    struct mgm_file m;
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening" << filename;
        return;
    }
    f.read((char*)(&m),sizeof(mgm_file));
    m.no_tiles = qFromBigEndian(m.no_tiles);
    qDebug() << "no_tiles:" << m.no_tiles;
}
