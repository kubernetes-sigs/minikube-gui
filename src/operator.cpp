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

#include "operator.h"
#include "paths.h"
#include "constants.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QStandardPaths>
#include <QDebug>

Operator::Operator(AdvancedView *advancedView, BasicView *basicView, ServiceView *serviceView,
                   AddonsView *addonsView, CommandRunner *commandRunner, ErrorMessage *errorMessage,
                   ProgressWindow *progressWindow, Tray *tray, HyperKit *hyperKit, Updater *updater,
                   QStackedWidget *stackedWidget, QDialog *parent)
{
    m_advancedView = advancedView;
    m_basicView = basicView;
    m_serviceView = serviceView;
    m_commandRunner = commandRunner;
    m_errorMessage = errorMessage;
    m_progressWindow = progressWindow;
    m_addonsView = addonsView;
    m_tray = tray;
    m_hyperKit = hyperKit;
    m_updater = updater;
    m_stackedWidget = stackedWidget;
    m_parent = parent;
    m_isBasicView = true;
    dashboardProcess = NULL;

    connect(m_basicView, &BasicView::start, this, &Operator::startMinikube);
    connect(m_basicView, &BasicView::stop, this, &Operator::stopMinikube);
    connect(m_basicView, &BasicView::pause, this, &Operator::pauseOrUnpauseMinikube);
    connect(m_basicView, &BasicView::delete_, this, &Operator::deleteMinikube);
    connect(m_basicView, &BasicView::refresh, this, &Operator::updateClusters);
    connect(m_basicView, &BasicView::dockerEnv, this, &Operator::dockerEnv);
    connect(m_basicView, &BasicView::service, this, &Operator::updateServices);
    connect(m_basicView, &BasicView::addons, this, &Operator::displayAddons);
    connect(m_basicView, &BasicView::mount, this, &Operator::mount);
    connect(m_basicView, &BasicView::closeMount, this, &Operator::mountClose);
    connect(m_basicView, &BasicView::tunnel, this, &Operator::tunnel);
    connect(m_basicView, &BasicView::ssh, this, &Operator::sshConsole);
    connect(m_basicView, &BasicView::dashboard, this, &Operator::dashboardBrowser);
    connect(m_basicView, &BasicView::advanced, this, &Operator::toAdvancedView);

    connect(m_advancedView, &AdvancedView::start, this, &Operator::startMinikube);
    connect(m_advancedView, &AdvancedView::stop, this, &Operator::stopMinikube);
    connect(m_advancedView, &AdvancedView::pause, this, &Operator::pauseOrUnpauseMinikube);
    connect(m_advancedView, &AdvancedView::delete_, this, &Operator::deleteMinikube);
    connect(m_advancedView, &AdvancedView::refresh, this, &Operator::updateClusters);
    connect(m_advancedView, &AdvancedView::dockerEnv, this, &Operator::dockerEnv);
    connect(m_advancedView, &AdvancedView::ssh, this, &Operator::sshConsole);
    connect(m_advancedView, &AdvancedView::dashboard, this, &Operator::dashboardBrowser);
    connect(m_advancedView, &AdvancedView::basic, this, &Operator::toBasicView);
    connect(m_advancedView, &AdvancedView::createCluster, this, &Operator::createCluster);
    connect(m_advancedView->clusterListView, SIGNAL(clicked(QModelIndex)), this,
            SLOT(updateButtons()));

    connect(m_commandRunner, &CommandRunner::startingExecution, this, &Operator::commandStarting);
    connect(m_commandRunner, &CommandRunner::executionEnded, this, &Operator::commandEnding);
    connect(m_commandRunner, &CommandRunner::output, this, &Operator::commandOutput);
    connect(m_commandRunner, &CommandRunner::error, this, &Operator::commandError);
    connect(m_commandRunner, &CommandRunner::updatedClusters, this, &Operator::clustersReceived);
    connect(m_commandRunner, &CommandRunner::updatedServices, this, &Operator::servicesReceived);
    connect(m_commandRunner, &CommandRunner::updatedAddons, this, &Operator::addonsReceived);
    connect(m_commandRunner, &CommandRunner::addonsComplete, this, &Operator::addonsComplete);
    connect(m_commandRunner, &CommandRunner::startCommandStarting, this,
            &Operator::startCommandStarting);

    connect(m_progressWindow, &ProgressWindow::cancelled, this, &Operator::cancelCommand);

    connect(m_tray, &Tray::restoreWindow, this, &Operator::restoreWindow);
    connect(m_tray, &Tray::hideWindow, this, &Operator::hideWindow);
    connect(m_tray, &Tray::start, this, &Operator::startMinikube);
    connect(m_tray, &Tray::stop, this, &Operator::stopMinikube);
    connect(m_tray, &Tray::pauseOrUnpause, this, &Operator::pauseOrUnpauseMinikube);

    connect(m_hyperKit, &HyperKit::rerun, this, &Operator::createCluster);

    connect(m_addonsView, &AddonsView::refresh, this, &Operator::updateAddons);
    connect(m_addonsView, &AddonsView::addonClicked, this, &Operator::addonsEnableDisable);

    updateClusters();
}

QStringList Operator::getCurrentClusterFlags()
{
    return { "-p", selectedClusterName() };
}

void Operator::startMinikube()
{
    m_commandRunner->startMinikube(getCurrentClusterFlags());
}

void Operator::stopMinikube()
{
    m_commandRunner->stopMinikube(getCurrentClusterFlags());
}

void Operator::pauseOrUnpauseMinikube()
{
    Cluster cluster = selectedCluster();
    if (cluster.status() == "Paused") {
        unpauseMinikube();
        return;
    }
    pauseMinikube();
}

void Operator::pauseMinikube()
{
    m_commandRunner->pauseMinikube(getCurrentClusterFlags());
}

void Operator::unpauseMinikube()
{
    m_commandRunner->unpauseMinikube(getCurrentClusterFlags());
}

void Operator::deleteMinikube()
{
    m_commandRunner->deleteMinikube(getCurrentClusterFlags());
}

void Operator::createCluster(QStringList args)
{
    m_commandRunner->startMinikube(args);
}

void Operator::startCommandStarting()
{
    commandStarting();
    m_progressWindow->setText("Starting...");
    m_progressWindow->show();
}

void Operator::commandStarting()
{
    m_advancedView->showLoading();
    m_tray->disableActions();
    m_parent->setCursor(Qt::WaitCursor);
    disableButtons();
}

void Operator::disableButtons()
{
    if (m_isBasicView) {
        m_basicView->disableButtons();
    } else {
        m_advancedView->disableButtons();
    }
}

void Operator::commandEnding()
{
    m_progressWindow->done();
    updateClusters();
}

void Operator::toAdvancedView()
{
    m_isBasicView = false;
    m_stackedWidget->setCurrentIndex(1);
    m_parent->resize(670, 400);
    updateButtons();
}

void Operator::toBasicView()
{
    m_isBasicView = true;
    m_stackedWidget->setCurrentIndex(0);
    m_parent->resize(Constants::basicViewWidth, Constants::basicViewHeight);
    updateButtons();
}

void Operator::updateClusters()
{
    m_commandRunner->requestClusters();
}

void Operator::updateServices()
{
    m_commandRunner->requestServiceList(selectedClusterName());
}

void Operator::updateAddons()
{
    m_commandRunner->requestAddons(selectedClusterName());
}

void Operator::displayAddons()
{
    updateAddons();
    m_addonsView->display();
}

void Operator::clustersReceived(ClusterList clusterList)
{
    m_clusterList = clusterList;
    m_advancedView->updateClustersTable(m_clusterList);
    updateButtons();
    m_advancedView->hideLoading();
    m_parent->unsetCursor();
    m_updater->checkForUpdates();
}

void Operator::servicesReceived(QString svcTable)
{
    m_serviceView->displayTable(svcTable);
    updateButtons();
    m_parent->unsetCursor();
}

void Operator::addonsReceived(AddonList as)
{
    m_addonsView->updateAddonsTable(as);
    updateButtons();
    m_parent->unsetCursor();
}

void Operator::updateButtons()
{
    Cluster cluster = selectedCluster();
    if (m_isBasicView) {
        m_basicView->update(cluster);
    } else {
        m_advancedView->update(cluster);
    }
    m_tray->updateTrayActions(cluster);
    m_tray->updateStatus(cluster);
}

void Operator::restoreWindow()
{
    bool wasVisible = m_parent->isVisible();
    m_parent->showNormal();
    m_parent->activateWindow();
    if (wasVisible) {
        return;
    }
    if (m_commandRunner->isRunning())
        return;
    updateClusters();
}

void Operator::hideWindow()
{
    m_parent->hide();
}

void Operator::commandOutput(QString text)
{
    QStringList lines;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    lines = text.split("\n", Qt::SkipEmptyParts);
#else
    lines = text.split("\n", QString::SkipEmptyParts);
#endif
    for (int i = 0; i < lines.size(); i++) {
        QJsonDocument json = QJsonDocument::fromJson(lines[i].toUtf8());
        QJsonObject object = json.object();
        QString type = object["type"].toString();
        if (type != "io.k8s.sigs.minikube.step") {
            return;
        }
        QJsonObject data = object["data"].toObject();
        QString stringStep = data["currentstep"].toString();
        int currStep = stringStep.toInt();
        QString totalString = data["totalsteps"].toString();
        int totalSteps = totalString.toInt();
        QString message = data["message"].toString();
        m_progressWindow->setBarMaximum(totalSteps);
        m_progressWindow->setBarValue(currStep);
        m_progressWindow->setText(message);
    }
}

void Operator::commandError(QStringList args, QString text)
{
#if __APPLE__
    if (m_hyperKit->hyperkitPermissionFix(args, text)) {
        return;
    }
#endif
    QStringList lines;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    lines = text.split("\n", Qt::SkipEmptyParts);
#else
    lines = text.split("\n", QString::SkipEmptyParts);
#endif
    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(line.toUtf8(), &error);
        if (json.isNull() || !json.isObject()) {
            continue;
        }
        QJsonObject par = json.object();
        QJsonObject data = par["data"].toObject();
        if (!data.contains("exitcode")) {
            continue;
        }
        QString advice = data["advice"].toString();
        QString message = data["message"].toString();
        QString name = data["name"].toString();
        QString url = data["url"].toString();
        QString issues = data["issues"].toString();

        m_errorMessage->error(name, advice, message, url, issues);
        break;
    }
}

void Operator::cancelCommand()
{
    m_commandRunner->stopCommand();
}

QString Operator::selectedClusterName()
{
    if (m_isBasicView) {
        return "minikube";
    }
    return m_advancedView->selectedClusterName();
}

Cluster Operator::selectedCluster()
{
    QString clusterName = selectedClusterName();
    if (clusterName.isEmpty()) {
        return Cluster();
    }
    ClusterList clusters = m_clusterList;
    ClusterHash clusterHash;
    for (int i = 0; i < clusters.size(); i++) {
        Cluster cluster = clusters.at(i);
        clusterHash[cluster.name()] = cluster;
    }
    return clusterHash[clusterName];
}

static QString minikubePath()
{
    QString minikubePath = QStandardPaths::findExecutable("minikube");
    if (!minikubePath.isEmpty()) {
        return minikubePath;
    }
    return QStandardPaths::findExecutable("minikube", Paths::minikubePaths());
}

void Operator::sshConsole()
{
    QString program = minikubePath();
    QString commandArgs = QString("ssh -p %1").arg(selectedClusterName());
    QString command = QString("%1 %2").arg(program, commandArgs);
#ifndef QT_NO_TERMWIDGET
    QMainWindow *mainWindow = new QMainWindow();
    int startnow = 0; // set shell program first

    QTermWidget *console = new QTermWidget(startnow);

    QFont font = QApplication::font();
    font.setFamily("Monospace");
    font.setPointSize(10);

    console->setTerminalFont(font);
    console->setColorScheme("Tango");
    console->setShellProgram(program);
    console->setArgs({ commandArgs });
    console->startShellProgram();

    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    mainWindow->setWindowTitle(nameLabel->text());
    mainWindow->resize(800, 400);
    mainWindow->setCentralWidget(console);
    mainWindow->show();
#elif __APPLE__
    QStringList arguments = { "-e", "tell app \"Terminal\"",
                              "-e", "do script \"" + command + "\"",
                              "-e", "activate",
                              "-e", "end tell" };
    m_commandRunner->executeCommand("/usr/bin/osascript", arguments);
#else
    QString terminal = qEnvironmentVariable("TERMINAL");
    if (terminal.isEmpty()) {
        terminal = "x-terminal-emulator";
        if (QStandardPaths::findExecutable(terminal).isEmpty()) {
            terminal = "xterm";
        }
    }

    m_commandRunner->executeCommand(QStandardPaths::findExecutable(terminal), { "-e", command });
#endif
}

void Operator::dockerEnv()
{
    QString program = minikubePath();
    QString commandArgs = QString("$(%1 -p %2 docker-env)").arg(program, selectedClusterName());
    QString command = QString("eval %1").arg(commandArgs);
#ifndef QT_NO_TERMWIDGET
    QMainWindow *mainWindow = new QMainWindow();
    int startnow = 0; // set shell program first

    QTermWidget *console = new QTermWidget(startnow);

    QFont font = QApplication::font();
    font.setFamily("Monospace");
    font.setPointSize(10);

    console->setTerminalFont(font);
    console->setColorScheme("Tango");
    console->setShellProgram("eval");
    console->setArgs({ commandArgs });
    console->startShellProgram();

    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    mainWindow->setWindowTitle(nameLabel->text());
    mainWindow->resize(800, 400);
    mainWindow->setCentralWidget(console);
    mainWindow->show();
#elif __APPLE__
    QStringList arguments = { "-e", "tell app \"Terminal\"",
                              "-e", "do script \"" + command + "\"",
                              "-e", "activate",
                              "-e", "end tell" };
    m_commandRunner->executeCommand("/usr/bin/osascript", arguments);
#else
    QString terminal = qEnvironmentVariable("TERMINAL");
    if (terminal.isEmpty()) {
        terminal = "x-terminal-emulator";
        if (QStandardPaths::findExecutable(terminal).isEmpty()) {
            terminal = "xterm";
        }
    }

    m_commandRunner->executeCommand(QStandardPaths::findExecutable(terminal), { "-e", command });
#endif
}

void Operator::mount(QString src, QString dest)
{
    mountClose();
    QProcess *process = new QProcess(this);
    QStringList arguments = { "-p", selectedClusterName(), src + ":" + dest };
    m_commandRunner->mountMinikube(arguments, process);

    mountProcess = process;
    mountProcess->waitForStarted();
    Mount m(src, dest, mountProcess);
    m_mountList << m;
    m_basicView->updateMounts(m_mountList);
}

void Operator::mountClose()
{
    if (mountProcess) {
        mountProcess->terminate();
        mountProcess->waitForFinished();
        m_mountList.removeAt(0);
    }
}

void Operator::tunnel()
{
    if (tunnelProcess) {
        return;
    }

    QProcess *process = new QProcess(this);
    QStringList arguments = { "-p", selectedClusterName() };
    m_commandRunner->tunnelMinikube(arguments);

    tunnelProcess = process;
    tunnelProcess->waitForFinished(-1);
    qDebug() << tunnelProcess->processId();
    qDebug() << tunnelProcess->readAllStandardOutput();
    tunnelProcess = NULL;
}

void Operator::addonsEnableDisable(QString addonName, QString action)
{
    QStringList arguments = { action, addonName, "-p", selectedClusterName() };
    m_commandRunner->addonsMinikube(arguments);
}

void Operator::addonsComplete()
{
    m_commandRunner->requestAddons(selectedClusterName());
}

void Operator::dashboardBrowser()
{
    dashboardClose();

    QProcess *process = new QProcess(this);
    QStringList arguments = { "-p", selectedClusterName() };
    m_commandRunner->dashboardMinikube(arguments, process);

    dashboardProcess = process;
    dashboardProcess->waitForStarted();
}

void Operator::dashboardClose()
{
    if (dashboardProcess) {
        dashboardProcess->terminate();
        dashboardProcess->waitForFinished();
    }
}
