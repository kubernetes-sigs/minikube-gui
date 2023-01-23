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

#include "basicview.h"
#include "fonts.h"
#include "constants.h"

#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QToolTip>
#include <QVBoxLayout>

BasicView::BasicView()
{
    basicView = new QWidget();
    startButton = new QPushButton(Constants::startIcon);
    stopButton = new QPushButton(Constants::stopIcon);
    pauseButton = new QPushButton(Constants::pauseIcon);
    deleteButton = new QPushButton(Constants::deleteIcon);
    refreshButton = new QPushButton(tr("Refresh GUI"));
    dockerEnvButton = new QPushButton("docker-env");
    sshButton = new QPushButton("SSH");
    dashboardButton = new QPushButton(tr("Dashboard"));
    advancedButton = new QPushButton(tr("Multi-cluster View"));

    Fonts::setFontAwesome(startButton);
    Fonts::setFontAwesome(stopButton);
    Fonts::setFontAwesome(pauseButton);
    Fonts::setFontAwesome(deleteButton);

    dockerEnvButton->setToolTip(
            tr("Opens a terminal where the docker-cli points to docker engine inside "
               "minikube\n(Useful for building docker images directly inside minikube)"));
    deleteButton->setToolTip(tr("Delete the default cluster"));
    stopButton->setToolTip(tr("Stop the default cluster"));

    disableButtons();

    QHBoxLayout *buttonLayoutRow1 = new QHBoxLayout;

    buttonLayoutRow1->addWidget(startButton);
    buttonLayoutRow1->addWidget(stopButton);
    buttonLayoutRow1->addWidget(pauseButton);
    buttonLayoutRow1->addWidget(deleteButton);

    QVBoxLayout *buttonLayoutRow2 = new QVBoxLayout;
    buttonLayoutRow2->addWidget(refreshButton);
    buttonLayoutRow2->addWidget(dockerEnvButton);
    buttonLayoutRow2->addWidget(sshButton);
    buttonLayoutRow2->addWidget(dashboardButton);
    buttonLayoutRow2->addWidget(advancedButton);

    QVBoxLayout *BasicLayout = new QVBoxLayout;
    BasicLayout->addLayout(buttonLayoutRow1);
    BasicLayout->addLayout(buttonLayoutRow2);
    basicView->setLayout(BasicLayout);

    basicView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    connect(startButton, &QPushButton::clicked, this, &BasicView::start);
    connect(stopButton, &QAbstractButton::clicked, this, &BasicView::stop);
    connect(pauseButton, &QAbstractButton::clicked, this, &BasicView::pause);
    connect(deleteButton, &QAbstractButton::clicked, this, &BasicView::delete_);
    connect(refreshButton, &QAbstractButton::clicked, this, &BasicView::refresh);
    connect(dockerEnvButton, &QAbstractButton::clicked, this, &BasicView::dockerEnv);
    connect(sshButton, &QAbstractButton::clicked, this, &BasicView::ssh);
    connect(dashboardButton, &QAbstractButton::clicked, this, &BasicView::dashboard);
    connect(advancedButton, &QAbstractButton::clicked, this, &BasicView::advanced);
}

static QString getPauseLabel(bool isPaused)
{
    if (isPaused) {
        return Constants::unPauseIcon;
    }
    return Constants::pauseIcon;
}

static QString getStartLabel(bool isRunning, bool isPaused)
{
    if (isRunning || isPaused) {
        return Constants::reloadIcon;
    }
    return Constants::startIcon;
}

static QString getPauseToolTip(bool isPaused)
{
    if (isPaused) {
        return "Unpause Kubernetes";
    }
    return "Pause Kubernetes cluster";
}

static QString getStartToolTip(bool isRunning, bool isPaused)
{
    if (isRunning || isPaused) {
        return "Restart (reconfigure) an already running cluster";
    }
    return "Start the default cluster";
}

void BasicView::update(Cluster cluster)
{
    startButton->setEnabled(true);
    advancedButton->setEnabled(true);
    refreshButton->setEnabled(true);
    bool exists = !cluster.isEmpty();
    bool isRunning = cluster.status() == "Running";
    bool isPaused = cluster.status() == "Paused";
    stopButton->setEnabled(isRunning || isPaused);
    pauseButton->setEnabled(isRunning || isPaused);
    deleteButton->setEnabled(exists);
    dashboardButton->setEnabled(isRunning);
#if __linux__ || __APPLE__
    dockerEnvButton->setEnabled(isRunning);
    sshButton->setEnabled(exists);
#else
    dockerEnvButton->setEnabled(false);
    sshButton->setEnabled(false);
#endif
    pauseButton->setText(getPauseLabel(isPaused));
    pauseButton->setToolTip(getPauseToolTip(isPaused));
    startButton->setText(getStartLabel(isRunning, isPaused));
    startButton->setToolTip(getStartToolTip(isRunning, isPaused));
}

void BasicView::disableButtons()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(false);
    deleteButton->setEnabled(false);
    pauseButton->setEnabled(false);
    dockerEnvButton->setEnabled(false);
    sshButton->setEnabled(false);
    dashboardButton->setEnabled(false);
    advancedButton->setEnabled(false);
    refreshButton->setEnabled(false);
}