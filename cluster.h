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

#ifndef CLUSTER_H
#define CLUSTER_H

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QMap>

class Cluster
{
public:
    Cluster() : Cluster("") { }
    Cluster(const QString &name)
        : m_name(name),
          m_status(""),
          m_driver(""),
          m_container_runtime(""),
          m_k8s_version(""),
          m_cpus(0),
          m_memory(0)
    {
    }

    QString name() const { return m_name; }
    QString status() const { return m_status; }
    void setStatus(QString status) { m_status = status; }
    QString driver() const { return m_driver; }
    void setDriver(QString driver) { m_driver = driver; }
    QString containerRuntime() const { return m_container_runtime; }
    void setContainerRuntime(QString containerRuntime) { m_container_runtime = containerRuntime; }
    QString k8sVersion() const { return m_k8s_version; }
    void setK8sVersion(QString k8sVersion) { m_k8s_version = k8sVersion; }
    int cpus() const { return m_cpus; }
    void setCpus(int cpus) { m_cpus = cpus; }
    int memory() const { return m_memory; }
    void setMemory(int memory) { m_memory = memory; }
    bool isEmpty() { return m_name.isEmpty(); }

private:
    QString m_name;
    QString m_status;
    QString m_driver;
    QString m_container_runtime;
    QString m_k8s_version;
    int m_cpus;
    int m_memory;
};

typedef QList<Cluster> ClusterList;
typedef QHash<QString, Cluster> ClusterHash;

class ClusterModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ClusterModel(const ClusterList &clusters, QObject *parent = nullptr)
        : QAbstractListModel(parent), clusterList(clusters)
    {
    }

    void setClusters(const ClusterList &clusters);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    ClusterList clusterList;
};

#endif // CLUSTER_H
