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
#include "mount.h"
#include "setting.h"
#include "linkbutton.h"

#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QVersionNumber>

class BasicView : public QObject
{
    Q_OBJECT

public:
    explicit BasicView(QIcon icon, QVersionNumber version);
    QWidget *basicView;
    void update(Cluster cluster);
    void updateMounts(MountList ms);
    void disableButtons();
    void setFont(QFont font, QWidget *wid);
    void receivedSettings(Setting s);
    void minikubeNotFound();
    void displayAbout();
signals:
    void start();
    void stop();
    void pause();
    void delete_();
    void refresh();
    void dockerEnv();
    void service();
    void ssh();
    void dashboard();
    void advanced();
    void mount(QString, QString);
    void saveSettings(Setting s);
    void closeMount();
    void tunnel();
    void addons();
    void openLink(QString);

private:
    QPushButton *topStatusButton;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *pauseButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;

    QPushButton *dockerEnvButton;
    QPushButton *serviceButton;
    QPushButton *mountButton;
    QPushButton *tunnelButton;
    QPushButton *sshButton;
    QPushButton *dashboardButton;
    QPushButton *addonsButton;

    LinkButton *dockerEnvLinkButton;
    LinkButton *serviceLinkButton;
    LinkButton *mountLinkButton;
    LinkButton *tunnelLinkButton;
    LinkButton *sshLinkButton;
    LinkButton *dashboardLinkButton;
    LinkButton *addonsLinkButton;

    QPushButton *advancedButton;
    QPushButton *settingsButton;
    QPushButton *aboutButton;
    QPushButton *exitButton;
    QIcon m_icon;
    QVersionNumber m_version;
    MountList m_mountList;
    Setting m_setting;
    void askMount();
    void askSettings();
};

#endif // BASICVIEW_H
