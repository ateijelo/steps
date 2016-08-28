/*
 * worldwindow.h -- part of Steps, a simple maps app
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

#ifndef WORLDWINDOW_H
#define WORLDWINDOW_H

#include <QWidget>

class WorldWindow : public QWidget
{
    Q_OBJECT
    public:
        WorldWindow(QWidget *parent = 0);
        void paintEvent(QPaintEvent *);
        void resize(const QSize& size);

    public slots:
        void zoomChanged(int z);

    private:
        void updateMask();
        int z;
};

#endif // WORLDWINDOW_H
