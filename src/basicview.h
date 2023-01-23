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

#ifndef BASICVIEW_H
#define BASICVIEW_H

#include "cluster.h"

#include <QObject>
#include <QPushButton>
#include <QLabel>

class BasicView : public QObject
{
    Q_OBJECT

public:
    explicit BasicView();
    QWidget *basicView;
    void update(Cluster cluster);
    void disableButtons();
    void setFont(QFont font, QWidget *wid);
signals:
    void start();
    void stop();
    void pause();
    void delete_();
    void refresh();
    void dockerEnv();
    void ssh();
    void dashboard();
    void advanced();

private:
    QLabel *topStatus;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *pauseButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
    QPushButton *dockerEnvButton;
    QPushButton *sshButton;
    QPushButton *dashboardButton;
    QPushButton *advancedButton;
};

#endif // BASICVIEW_H
