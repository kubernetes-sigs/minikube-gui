#include "addon.h"

#include <QString>

#include <QStringList>

void AddonModel::setAddonList(const AddonList &addons)
{
    beginResetModel();
    addonList = addons;
    endResetModel();
}

int AddonModel::rowCount(const QModelIndex &) const
{
    return addonList.count();
}

int AddonModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant AddonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= addonList.size())
        return QVariant();
    if (index.column() >= 3)
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 0:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case 1:
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        case 2:
            // fall-through
            return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
    if (role == Qt::DisplayRole) {
        Addon addon = addonList.at(index.row());
        switch (index.column()) {
        case 0:
            return addon.name();
        case 1:
            return addon.status();
        case 2:
            return "Enable/Disable";
        }
    }
    return QVariant();
}

QVariant AddonModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return tr("Action");
        }
    }

    return QVariant(); // QStringLiteral("Row %1").arg(section);
}
