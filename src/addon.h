/*
Copyright 2023 The Kubernetes Authors All rights reserved.

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

#ifndef ADDON_H
#define ADDON_H

#include <QAbstractListModel>

class Addon
{
public:
    Addon(QString name, QString status) : m_name(name), m_status(status) {};
    Addon() : Addon("", "") {};
    QString name() const { return m_name; }
    QString status() const { return m_status; }

private:
    QString m_name;
    QString m_status;
};

typedef QList<Addon> AddonList;

class AddonModel : public QAbstractListModel
{
    Q_OBJECT

public:
    AddonModel(const AddonList &addons, QObject *parent = nullptr)
        : QAbstractListModel(parent), addonList(addons)
    {
    }

    void setAddonList(const AddonList &addons);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    AddonList addonList;
};

#endif // ADDON_H
