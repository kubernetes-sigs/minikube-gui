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

#include <QVBoxLayout>

#include <QFontDatabase>
#include <QFont>
#include <QDebug>
#include <QChar>

const QString startIcon = "\uf04b";
const QString stopIcon = "\uf04d";
const QString pauseIcon = "\uf04c";
const QString deleteIcon = "\uf1f8";
const QString reloadIcon = "\uf021";

void BasicView::setFA(QWidget *wid)
    {
    if (QFontDatabase::addApplicationFont(":/images/FontAwesome.otf") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
    QFont font;
    font.setFamily("FontAwesome");
    font.setPixelSize(20);
    wid->setFont(font);
    }
BasicView::BasicView()
{
    basicView = new QWidget();
    startButton = new QPushButton(startIcon);
    setFA(startButton);

    stopButton = new QPushButton(stopIcon);
    setFA(stopButton);

    pauseButton = new QPushButton(pauseIcon);
    setFA(pauseButton);

    deleteButton = new QPushButton(deleteIcon);
    setFA(deleteButton);

    refreshButton = new QPushButton(tr("Refresh"));
    dockerEnvButton = new QPushButton(tr("docker-env"));
    dockerEnvButton->setToolTip(
            "Opens a terminal where the docker-cli points to docker engine inside "
            "minikube\n(Useful for building docker images directly inside minikube)");
    sshButton = new QPushButton(tr("SSH"));
    dashboardButton = new QPushButton(tr("Dashboard"));
    advancedButton = new QPushButton(tr("Multi-cluster View"));

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
        return "Unpause";
    }
    return "\uf04c";
}



static QString getStartLabel(bool isRunning)
{
    if (isRunning) {
        return reloadIcon;
    }
    return startIcon;
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
    startButton->setText(getStartLabel(isRunning));
    QString startToolTip = "";
    if (isRunning) {
        startToolTip = "Restart an already running minikube instance to pickup config changes.";
    }
    startButton->setToolTip(startToolTip);
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
