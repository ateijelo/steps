#include <QtDebug>
#include "pathsdockmodel.h"

PathsDockModel::PathsDockModel()
{

}

void PathsDockModel::addPath(const Path *p)
{
    paths.append(p);
}

QModelIndex PathsDockModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row, column, row);
}

QModelIndex PathsDockModel::parent(const QModelIndex &child) const
{
    qDebug() << "calling parent for" << child;
    return QModelIndex();
}

int PathsDockModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "rowCount returned " << paths.count();
    //return paths.count();
    return 3;
}

int PathsDockModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant PathsDockModel::data(const QModelIndex &index, int role) const
{
    switch (index.column()) {
    case 0:
        //return paths.at(index.row())->name();
        return "prueba";
    case 1:
        //return paths.at(index.row())->isVisible();
        return true;
    case 2:
        //return paths.at(index.row())->length();
        return 4.58;
    }
    return QVariant();
}

