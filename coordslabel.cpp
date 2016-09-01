/*
 * coordslabel.cpp -- part of Steps, a simple maps app
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

#include <QSettings>


#include <math.h>

#include "constants.h"
#include "coordslabel.h"

CoordsLabel::CoordsLabel(QWidget *parent)
    : QLabel(parent)
{
    QSettings settings;
    show_dms = settings.value(SettingsKeys::CoordsLabelShowsDMS, true).toBool();
    coordsTemplate = QString::fromUtf8(" %1°%2'%3\"%4 %5°%6'%7\"%8 ");
}

void CoordsLabel::setCoords(qreal lat, qreal lon)
{
    this->lat = lat;
    this->lon = lon;
    updateCoords();
}

void CoordsLabel::updateCoords()
{
    char ns = (lat > 0)? 'N' : 'S';
    char ew = (lon > 0)? 'E' : 'W';

    if (show_dms) {
        double a1 = fabs(lat);
        double d1 = floor(a1);
        double m1 = floor(fmod(a1*60,60));
        double s1 = fmod(a1*3600,60);

        double a2 = fabs(lon);
        double d2 = floor(a2);
        double m2 = floor(fmod(a2*60,60));
        double s2 = fmod(a2*3600,60);

        setText(coordsTemplate
            .arg(d1,0,'f',0,' ')
            .arg(m1,2,'f',0,'0')
            .arg(s1,5,'f',2,'0').arg(ns)
            .arg(d2,0,'f',0,' ')
            .arg(m2,2,'f',0,'0')
            .arg(s2,5,'f',2,'0').arg(ew));
    } else {
        setText(QString("%1 %2 %3 %4")
                .arg(lat,0,'f',6)
                .arg(ns)
                .arg(lon,0,'f',6)
                .arg(ew));
    }
}

void CoordsLabel::mousePressEvent(QMouseEvent *e)
{
    QSettings settings;
    if (e->button() == Qt::LeftButton) {
        show_dms = !show_dms;
        settings.setValue(SettingsKeys::CoordsLabelShowsDMS, show_dms);
        updateCoords();
    }
}
