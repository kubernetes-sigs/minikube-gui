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

#ifndef COMMANDRUNNER_H
#define COMMANDRUNNER_H

#include "cluster.h"
#include "addon.h"
#include "logger.h"
#include "settings.h"

#include <QDialog>
#include <QProcessEnvironment>

class CommandRunner : public QObject
{
    Q_OBJECT

public:
    CommandRunner(QDialog *parent, Logger *logger, Settings *settings);

    void executeCommand(QString program, QStringList args);
    void startMinikube(QStringList args);
    void stopMinikube(QStringList args);
    void pauseMinikube(QStringList args);
    void unpauseMinikube(QStringList args);
    void deleteMinikube(QStringList args);
    void mountMinikube(QStringList args, QProcess *process);
    void tunnelMinikube(QStringList args);
    void addonsMinikube(QStringList args);
    void dashboardMinikube(QStringList args, QProcess *process);
    void stopCommand();
    void requestClusters();
    void requestServiceList(QString profName);
    void requestAddons(QString profName);
    bool isRunning();

signals:
    void startingExecution();
    void executionEnded();
    void output(QString text);
    void error(QStringList args, QString text);
    void updatedClusters(ClusterList clusterList);
    void updatedServices(QString);
    void updatedAddons(AddonList);
    void startCommandStarting();
    void addonsComplete();
    void minikubeNotFound();

private slots:
    void executionCompleted();
    void outputReady();
    void errorReady();

private:
    void executeMinikubeCommand(QStringList args);
    void executeMinikubeCommand(QStringList args, QProcess *process);
    QString minikubePath();
    void errorHappened(QProcess::ProcessError);
#if __APPLE__
    void setMinikubePath();
#endif

    QProcess *m_process;
    QProcessEnvironment m_env;
    QString m_output;
    QString m_command;
    QDialog *m_parent;
    Logger *m_logger;
    Settings *m_settings;
    QStringList m_args;
    bool m_isRunning;
};

#endif // COMMANDRUNNER_H
