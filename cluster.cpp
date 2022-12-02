/*
Copyright 2022 The Kubernetes Authors All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "cluster.h"

#include <QStringList>

void ClusterModel::setClusters(const ClusterList &clusters)
{
    beginResetModel();
    clusterList = clusters;
    endResetModel();
}

int ClusterModel::rowCount(const QModelIndex &) const
{
    return clusterList.count();
}

int ClusterModel::columnCount(const QModelIndex &) const
{
    return 7;
}

static QStringList binaryAbbrs = { "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB" };

QVariant ClusterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= clusterList.size())
        return QVariant();
    if (index.column() >= 7)
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 0:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case 1:
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        case 2:
            // fall-through
        case 3:
            // fall-through
        case 4:
            // fall-through
        case 5:
            // fall-through
        case 6:
            return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
    if (role == Qt::DisplayRole) {
        Cluster cluster = clusterList.at(index.row());
        switch (index.column()) {
        case 0:
            return cluster.name();
        case 1:
            return cluster.status();
        case 2:
            return cluster.driver();
        case 3:
            return cluster.containerRuntime();
        case 4:
            return cluster.k8sVersion();
        case 5:
            return QString::number(cluster.cpus());
        case 6:
            return QString::number(cluster.memory());
        }
    }
    return QVariant();
}

QVariant ClusterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Status");
        case 2:
            return tr("Driver");
        case 3:
            return tr("Container Runtime");
        case 4:
            return tr("Kubernetes Version");
        case 5:
            return tr("CPUs");
        case 6:
            return tr("Memory (MB)");
        }
    }
    return QVariant(); // QStringLiteral("Row %1").arg(section);
}
