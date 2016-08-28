/*
 * worldwindow.cpp -- part of Steps, a simple maps app
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
#include <QRegion>
#include <QPainter>
#include <QMouseEvent>

#include "geotools.h"
#include "worldwindow.h"

#include "debug.h"

WorldWindow::WorldWindow(QWidget *parent)
    : QWidget(parent)
{
    //setAttribute(Qt::WA_TransparentForMouseEvents);
    updateMask();
}

void WorldWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    if (fEnabled(DEBUG_WORLDWINDOW))
        p.setBrush(QBrush(QColor(255,0,0,128)));
    else
        p.setBrush(Qt::white);
    p.drawRect(rect());
}

void WorldWindow::resize(const QSize &size)
{
    QWidget::resize(size);
    updateMask();
}

//void WorldWindow::resizeEvent(QResizeEvent *event)
//{
//    qDebug() << "WorldWindow::resizeEvent";
//    qDebug() << "    event:" << event;
//    QWidget::resizeEvent(event);
//    updateMask();
//}

void WorldWindow::zoomChanged(int z)
{
    this->z = z;
    updateMask();
}

void WorldWindow::updateMask()
{
    qDebug() << "WorldWindow::updateMask";
    qDebug() << "    width:" << width();
    qDebug() << "    zoom:" << z;

    int worldPixelWidth = 256 * (1 << qBound(0,z,20));
    int worldPixelHeight = worldPixelWidth;
    if (worldPixelWidth > width() && worldPixelHeight > height())
    {
        hide();
        return;
    }
    show();
    int x1 = (width() - worldPixelWidth)/2;
    int x2 = x1 + worldPixelWidth;
    int y1 = (height() - worldPixelHeight)/2;
    int y2 = y1 + worldPixelHeight;

    QRegion r(QRect(0,0,x1,height()));
    r = r.united(QRect(x2,0,width(),height()));
    r = r.united(QRect(x1,0,worldPixelWidth,y1));
    r = r.united(QRect(x1,y2,worldPixelWidth,height() - y2));
//    QTransform t;
//    t.translate(width()/2,height()/2);
//    t.rotate(45);
//    t.translate(-width()/2,-height()/2);
//    r = t.map(r);
    setMask(r);
}
