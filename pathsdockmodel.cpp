#include <QtDebug>
#include "pathsdockmodel.h"

PathsDockModel::PathsDockModel()
{
    pathLengthsMapper = new QSignalMapper(this);
    connect(pathLengthsMapper, SIGNAL(mapped(int)),
            this, SLOT(pathLengthChanged(int)));
}

void PathsDockModel::addPath(const Path *p)
{
    beginInsertRows(QModelIndex(), paths.count(), paths.count());
    connect(p, SIGNAL(lengthChanged(double)), pathLengthsMapper, SLOT(map()));
    pathLengthsMapper->setMapping((QObject*)p, paths.count());
    paths.append(p);
    endInsertRows();
}

QModelIndex PathsDockModel::index(int row, int column, const QModelIndex &parent) const
{
    //if (parent.isValid())
    //    return QModelIndex();
    return createIndex(row, column, row);
}

QModelIndex PathsDockModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int PathsDockModel::rowCount(const QModelIndex &parent) const
{
    //return paths.count();
    if (!parent.isValid())
        return paths.count();
    return 0;
}

int PathsDockModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant PathsDockModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (index.column()) {
    case 0:
        return paths.at(index.row())->name();
    case 1:
        qDebug() << "returning length for row" << index.row() << ":" << paths.at(index.row())->length();
        return paths.at(index.row())->length();
    case 2:
        return paths.at(index.row())->isVisible();
    }
    return QVariant();
}

void PathsDockModel::pathLengthChanged(int row)
{
    qDebug() << "PathsDockModel::pathLengthChanged(" << row << ")";
    emit dataChanged(createIndex(row, 1, row),
                     createIndex(row, 1, row));
}

