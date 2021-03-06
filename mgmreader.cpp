/*
 * mgmreader.cpp -- part of Steps, a simple maps app
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
