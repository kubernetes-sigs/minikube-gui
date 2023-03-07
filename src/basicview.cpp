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

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>

BasicView::BasicView(QIcon icon)
{
    m_icon = icon;
    basicView = new QWidget();

    topStatus = new QLabel("Loading ...");
    QVBoxLayout *topBar = new QVBoxLayout;
    topStatus->setAlignment(Qt::AlignCenter);

    topBar->addWidget(topStatus);

    startButton = new QPushButton(Constants::startIcon);
    stopButton = new QPushButton(Constants::stopIcon);
    pauseButton = new QPushButton(Constants::pauseIcon);
    deleteButton = new QPushButton(Constants::deleteIcon);
    refreshButton = new QPushButton(tr("refresh gui"));
    dockerEnvButton = new QPushButton("docker-env");
    serviceButton = new QPushButton("service");
    mountButton = new QPushButton(tr("mount"));
    tunnelButton = new QPushButton(tr("tunnel"));
    sshButton = new QPushButton("SSH");
    dashboardButton = new QPushButton(tr("dashboard"));
    addonsButton = new QPushButton(tr("addons"));
    advancedButton = new QPushButton(tr("cluster list"));
    settingsButton = new QPushButton("gui settings");

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
    buttonLayoutRow2->addWidget(serviceButton);
    buttonLayoutRow2->addWidget(mountButton);
    buttonLayoutRow2->addWidget(tunnelButton);
    buttonLayoutRow2->addWidget(sshButton);
    buttonLayoutRow2->addWidget(dashboardButton);
    buttonLayoutRow2->addWidget(addonsButton);
    buttonLayoutRow2->addWidget(advancedButton);
    buttonLayoutRow2->addWidget(settingsButton);

    QVBoxLayout *BasicLayout = new QVBoxLayout;
    BasicLayout->addLayout(topBar);
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
    connect(serviceButton, &QPushButton::clicked, this, &BasicView::service);
    connect(mountButton, &QAbstractButton::clicked, this, &BasicView::askMount);
    connect(tunnelButton, &QAbstractButton::clicked, this, &BasicView::tunnel);
    connect(sshButton, &QAbstractButton::clicked, this, &BasicView::ssh);
    connect(dashboardButton, &QAbstractButton::clicked, this, &BasicView::dashboard);
    connect(addonsButton, &QAbstractButton::clicked, this, &BasicView::addons);
    connect(advancedButton, &QAbstractButton::clicked, this, &BasicView::advanced);
    connect(settingsButton, &QAbstractButton::clicked, this, &BasicView::askSettings);
}

static QString getPauseLabel(bool isPaused)
{
    if (isPaused) {
        return Constants::unPauseIcon;
    }
    return Constants::pauseIcon;
}

static QString getStartLabel(bool exists, bool isRunning, bool isPaused)
{
    if (!exists) {
        return Constants::createIcon;
    }

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

static QString getStartToolTip(bool exists, bool isRunning, bool isPaused)
{
    if (!exists) {
        return "Create a new cluster";
    }

    if (isRunning || isPaused) {
        return "Restart (reconfigure) an already running cluster";
    }
    return "Start the cluster";
}

// updates clusters (TODO: change name)
void BasicView::update(Cluster cluster)
{
    startButton->setEnabled(true);
    advancedButton->setEnabled(true);
    refreshButton->setEnabled(true);
    bool exists = !cluster.isEmpty();
    bool isRunning = cluster.status() == "Running";
    bool isPaused = cluster.status() == "Paused";
    topStatus->setText(cluster.status());
    serviceButton->setEnabled(isRunning || isPaused);
    stopButton->setEnabled(isRunning || isPaused);
    pauseButton->setEnabled(isRunning || isPaused);
    deleteButton->setEnabled(exists);
    dashboardButton->setEnabled(isRunning);
    addonsButton->setEnabled(isRunning);
#if __linux__ || __APPLE__
    dockerEnvButton->setEnabled(isRunning || isPaused);
    sshButton->setEnabled(isRunning || isPaused);
    mountButton->setEnabled(isRunning || isPaused);
    tunnelButton->setEnabled(isRunning || isPaused);
#else
    dockerEnvButton->setEnabled(false);
    sshButton->setEnabled(false);
    mountButton->setEnabled(false);
    tunnelButton->setEnabled(false);
#endif
    pauseButton->setText(getPauseLabel(isPaused));
    pauseButton->setToolTip(getPauseToolTip(isPaused));
    startButton->setText(getStartLabel(exists, isRunning, isPaused));
    startButton->setToolTip(getStartToolTip(exists, isRunning, isPaused));
}

void BasicView::updateMounts(MountList ms)
{
    m_mountList = ms;
}

void BasicView::disableButtons()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(false);
    deleteButton->setEnabled(false);
    pauseButton->setEnabled(false);
    dockerEnvButton->setEnabled(false);
    serviceButton->setEnabled(false);
    tunnelButton->setEnabled(false);
    mountButton->setEnabled(false);
    sshButton->setEnabled(false);
    dashboardButton->setEnabled(false);
    addonsButton->setEnabled(false);
    advancedButton->setEnabled(false);
    refreshButton->setEnabled(false);
}

void BasicView::askMount()
{

    QDialog dialog;
    dialog.setWindowIcon(m_icon);
    dialog.setModal(true);

    // New Mount
    if (m_mountList.length() < 1) {
        dialog.setWindowTitle(tr("Create A new mount"));
        QFormLayout form(&dialog);
        QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
        QLineEdit srcField(&dialog);
        QLineEdit destField(&dialog);
        form.addRow(new QLabel(tr("src")), &srcField);
        form.addRow(new QLabel(tr("dest")), &destField);
        buttonBox.addButton(QString(tr("Start mount")), QDialogButtonBox::AcceptRole);
        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        buttonBox.addButton(QString(tr("Cancel")), QDialogButtonBox::RejectRole);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        form.addRow(&buttonBox);

        int code = dialog.exec();
        if (code == QDialog::Accepted) {
            emit mount(srcField.text(), destField.text());
        }

    } else {
        QProcess *m_proc = m_mountList[0].proc();
        QString allLogs = m_mountList[0].logs();
        QString newLog = m_proc->readAllStandardOutput();
        allLogs = allLogs + newLog;
        m_mountList[0].setLogs(allLogs);
        dialog.setWindowTitle(tr("Already mounted"));
        QFormLayout form(&dialog);
        QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
        form.addRow(new QLabel(allLogs));
        buttonBox.addButton(QString(tr("Ok ! ")), QDialogButtonBox::RejectRole);
        buttonBox.addButton(QString(tr("Stop Mount! ")), QDialogButtonBox::AcceptRole);
        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        form.addRow(&buttonBox);
        int code = dialog.exec();
        if (code == QDialog::Accepted) {
            m_mountList.removeAt(0);
            m_proc->terminate();
        }
    }
}

void BasicView::askSettings()
{

    QDialog dialog;
    dialog.setWindowIcon(m_icon);
    dialog.setModal(true);

    dialog.setWindowTitle(tr("GUI Settings"));
    QFormLayout form(&dialog);
    QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
    QLineEdit binaryPath(&dialog);
    form.addRow(new QLabel(tr("path to minikube binary")), &binaryPath);
    buttonBox.addButton(QString(tr("save")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    buttonBox.addButton(QString(tr("Cancel")), QDialogButtonBox::RejectRole);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow(&buttonBox);

    int code = dialog.exec();
    if (code == QDialog::Accepted) {
        emit sendSettngs(binaryPath.text());
    }


}

