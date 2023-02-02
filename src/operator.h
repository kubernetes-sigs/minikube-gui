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

#ifndef OPERATOR_H
#define OPERATOR_H

#include "advancedview.h"
#include "basicview.h"
#include "cluster.h"
#include "commandrunner.h"
#include "errormessage.h"
#include "hyperkit.h"
#include "progresswindow.h"
#include "tray.h"
#include "updater.h"
#include "mount.h"
#include <QStackedWidget>

class Operator : public QObject
{
    Q_OBJECT

public:
    Operator(AdvancedView *advancedView, BasicView *basicView, CommandRunner *commandRunner,
             ErrorMessage *errorMessage, ProgressWindow *progressWindow, Tray *tray,
             HyperKit *hyperKit, Updater *updater, QStackedWidget *stackedWidget, QDialog *parent);

public slots:
    void startMinikube();
    void stopMinikube();
    void pauseOrUnpauseMinikube();
    void deleteMinikube();

private slots:
    void commandStarting();
    void commandEnding();
    void commandOutput(QString text);
    void commandError(QStringList args, QString text);
    void cancelCommand();
    void toBasicView();
    void toAdvancedView();
    void createCluster(QStringList args);
    void updateButtons();
    void clustersReceived(ClusterList clusterList);
    void startCommandStarting();

private:
    QStringList getCurrentClusterFlags();
    void updateClusters();
    QString selectedClusterName();
    Cluster selectedCluster();
    void sshConsole();
    void dockerEnv();
    void mount(QString, QString);
    void dashboardBrowser();
    void dashboardClose();
    void mountClose();
    void pauseMinikube();
    void unpauseMinikube();
    void restoreWindow();
    void hideWindow();
    void disableButtons();

    AdvancedView *m_advancedView;
    BasicView *m_basicView;
    CommandRunner *m_commandRunner;
    ErrorMessage *m_errorMessage;
    ProgressWindow *m_progressWindow;
    ClusterList m_clusterList;
    Tray *m_tray;
    HyperKit *m_hyperKit;
    Updater *m_updater;
    bool m_isBasicView;
    QProcess *dashboardProcess;
    QProcess *mountProcess;
    QStackedWidget *m_stackedWidget;
    QDialog *m_parent;
    MountList m_mountList;
};

#endif // OPERATOR_H
