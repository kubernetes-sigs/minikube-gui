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

#ifndef ADVANCEDVIEW_H
#define ADVANCEDVIEW_H

#include "cluster.h"
#include "minikubebutton.h"

#include <QLabel>
#include <QPushButton>
#include <QTableView>

class AdvancedView : public QObject
{
    Q_OBJECT

public:
    explicit AdvancedView(QIcon icon);
    QWidget *advancedView;
    QTableView *clusterListView;

    QString selectedClusterName();
    void updateClustersTable(ClusterList clusters);
    void showLoading();
    void hideLoading();
    void disableButtons();

public slots:
    void update(Cluster cluster);

signals:
    void start();
    void stop();
    void pause();
    void delete_();
    void refresh();
    void dockerEnv();
    void ssh();
    void dashboard();
    void basic();
    void createCluster(QStringList args);

private:
    void setSelectedClusterName(QString cluster);
    void askName();
    void askCustom();

    MinikubeButton *startButton;
    MinikubeButton *stopButton;
    MinikubeButton *pauseButton;
    MinikubeButton *deleteButton;
    MinikubeButton *refreshButton;
    MinikubeButton *dockerEnvButton;
    MinikubeButton *sshButton;
    MinikubeButton *dashboardButton;
    MinikubeButton *basicButton;
    MinikubeButton *createButton;
    QLabel *loading;
    ClusterModel *m_clusterModel;

    QIcon m_icon;
};

#endif // ADVANCEDVIEW_H
