#ifndef PATHSDOCKMODEL_H
#define PATHSDOCKMODEL_H

#include <QList>
#include <QAbstractItemModel>

#include "path.h"

class PathsDockModel : public QAbstractItemModel
{
public:
    PathsDockModel();

    void addPath(const Path* p);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QList<Path const *> paths;
};

#endif // PATHSDOCKMODEL_H
