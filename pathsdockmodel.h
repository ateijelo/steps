/*
 * pathsdockmodel.h -- part of Steps, a simple maps app
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

#ifndef PATHSDOCKMODEL_H
#define PATHSDOCKMODEL_H


#include <QList>
#include <QSignalMapper>
#include <QAbstractItemModel>

#include "path.h"

class PathsDockModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    PathsDockModel();

    void addPath(const Path* p);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private slots:
    void pathLengthChanged(int row);

private:
    QList<Path const *> paths;
    QSignalMapper *pathLengthsMapper;
};

#endif // PATHSDOCKMODEL_H
