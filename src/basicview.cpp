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
#include "linkbutton.h"

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QApplication>
#include <QCoreApplication>
#include <QCheckBox>

BasicView::BasicView(QIcon icon,QVersionNumber version)
{
    m_icon = icon;
    m_version = version;
    basicView = new QWidget();

    topStatusButton = new QPushButton("Loading ...");
    topStatusButton->setMouseTracking(true);
    topStatusButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    QVBoxLayout *topBar = new QVBoxLayout;
    topBar->addWidget(topStatusButton);

    startButton = new QPushButton(Constants::startIcon);
    stopButton = new QPushButton(Constants::stopIcon);
    pauseButton = new QPushButton(Constants::pauseIcon);
    deleteButton = new QPushButton(Constants::deleteIcon);

    dockerEnvButton = new QPushButton("docker-env");
    serviceButton = new QPushButton("service");
    mountButton = new QPushButton(tr("mount"));
    tunnelButton = new QPushButton(tr("tunnel"));
    sshButton = new QPushButton("SSH");
    dashboardButton = new QPushButton(tr("dashboard"));
    addonsButton = new QPushButton(tr("addons"));
    advancedButton = new QPushButton(tr("cluster list"));

    dockerEnvLinkButton = new LinkButton(Constants::linkIcon, "https://minikube.sigs.k8s.io/docs/commands/docker-env/");
    serviceLinkButton = new LinkButton(Constants::linkIcon, "https://minikube.sigs.k8s.io/docs/commands/service/");
    mountLinkButton = new LinkButton(Constants::linkIcon, "https://minikube.sigs.k8s.io/docs/commands/mount/");
    tunnelLinkButton = new LinkButton(Constants::linkIcon, "https://minikube.sigs.k8s.io/docs/commands/tunnel/");
    sshLinkButton = new LinkButton(Constants::linkIcon, "https://minikube.sigs.k8s.io/docs/commands/ssh/");
    dashboardLinkButton = new LinkButton(Constants::linkIcon, "https://minikube.sigs.k8s.io/docs/commands/dashboard/");
    addonsLinkButton = new LinkButton(Constants::linkIcon, "https://minikube.sigs.k8s.io/docs/commands/addons/");

    refreshButton = new QPushButton(Constants::refreshIcon);
    settingsButton = new QPushButton(Constants::settingsIcon);
    aboutButton = new QPushButton(Constants::aboutIcon);
    exitButton = new QPushButton(Constants::exitIcon);

    // all the buttons that have icon needs to be set here
    Fonts::setFontAwesome(startButton);
    Fonts::setFontAwesome(stopButton);
    Fonts::setFontAwesome(pauseButton);
    Fonts::setFontAwesome(deleteButton);
    Fonts::setFontAwesome(refreshButton);
    Fonts::setFontAwesome(settingsButton);
    Fonts::setFontAwesome(aboutButton);
    Fonts::setFontAwesome(exitButton);

    Fonts::setFontAwesome(dockerEnvLinkButton);
    Fonts::setFontAwesome(serviceLinkButton);
    Fonts::setFontAwesome(mountLinkButton);
    Fonts::setFontAwesome(tunnelLinkButton);
    Fonts::setFontAwesome(sshLinkButton);
    Fonts::setFontAwesome(dashboardLinkButton);
    Fonts::setFontAwesome(addonsLinkButton);

    topStatusButton->setToolTip(tr("cluster status, click to refresh"));
    dockerEnvButton->setToolTip(
            tr("Opens a terminal where the docker-cli points to docker engine inside "
               "minikube\n(Useful for building docker images directly inside minikube)"));
    deleteButton->setToolTip(tr("Delete the default cluster"));
    stopButton->setToolTip(tr("Stop the default cluster"));
    settingsButton->setToolTip(tr("minikube-gui settings"));
    aboutButton->setToolTip(tr("about"));
    exitButton->setToolTip(tr("exit"));

    disableButtons();

    QHBoxLayout *buttonLayoutRow1 = new QHBoxLayout;

    buttonLayoutRow1->addWidget(startButton);
    buttonLayoutRow1->addWidget(stopButton);
    buttonLayoutRow1->addWidget(pauseButton);
    buttonLayoutRow1->addWidget(deleteButton);

    QVBoxLayout *buttonLayoutRow2 = new QVBoxLayout;

#define addRow(btn, linkbtn) do{        \
    QHBoxLayout *row = new QHBoxLayout; \
    row->addWidget(btn);                \
    row->addWidget(linkbtn);            \
    buttonLayoutRow2->addLayout(row);   \
}while(0)

    addRow(dockerEnvButton, dockerEnvLinkButton);
    addRow(serviceButton, serviceLinkButton);
    addRow(mountButton, mountLinkButton);
    addRow(tunnelButton, tunnelLinkButton);
    addRow(sshButton, sshLinkButton);
    addRow(dashboardButton, dashboardLinkButton);
    addRow(addonsButton, addonsLinkButton);
    buttonLayoutRow2->addWidget(advancedButton);

#undef addRow

    QHBoxLayout *bottomBar = new QHBoxLayout;
    bottomBar->addWidget(settingsButton);
    bottomBar->addWidget(aboutButton);
    bottomBar->addWidget(exitButton);

    //  add all layouts to the basic view
    QVBoxLayout *BasicLayout = new QVBoxLayout;
    BasicLayout->addLayout(topBar);
    BasicLayout->addLayout(buttonLayoutRow1);
    BasicLayout->addLayout(buttonLayoutRow2);
    BasicLayout->addLayout(bottomBar);
    basicView->setLayout(BasicLayout);

    basicView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    connect(topStatusButton, &QPushButton::clicked, this, &BasicView::refresh);
    connect(startButton, &QPushButton::clicked, this, &BasicView::start);
    connect(stopButton, &QAbstractButton::clicked, this, &BasicView::stop);
    connect(pauseButton, &QAbstractButton::clicked, this, &BasicView::pause);
    connect(deleteButton, &QAbstractButton::clicked, this, &BasicView::delete_);

    connect(dockerEnvButton, &QAbstractButton::clicked, this, &BasicView::dockerEnv);
    connect(serviceButton, &QPushButton::clicked, this, &BasicView::service);
    connect(mountButton, &QAbstractButton::clicked, this, &BasicView::askMount);
    connect(tunnelButton, &QAbstractButton::clicked, this, &BasicView::tunnel);
    connect(sshButton, &QAbstractButton::clicked, this, &BasicView::ssh);
    connect(dashboardButton, &QAbstractButton::clicked, this, &BasicView::dashboard);
    connect(addonsButton, &QAbstractButton::clicked, this, &BasicView::addons);
    connect(advancedButton, &QAbstractButton::clicked, this, &BasicView::advanced);
    connect(settingsButton, &QAbstractButton::clicked, this, &BasicView::askSettings);
    connect(aboutButton, &QAbstractButton::clicked, this, &BasicView::displayAbout);
    connect(exitButton, &QAbstractButton::clicked, qApp, &QCoreApplication::quit);

    connect(dockerEnvLinkButton, &LinkButton::openLink, this, &BasicView::openLink);
    connect(serviceLinkButton, &LinkButton::openLink, this, &BasicView::openLink);
    connect(mountLinkButton, &LinkButton::openLink, this, &BasicView::openLink);
    connect(tunnelLinkButton, &LinkButton::openLink, this, &BasicView::openLink);
    connect(sshLinkButton, &LinkButton::openLink, this, &BasicView::openLink);
    connect(dashboardLinkButton, &LinkButton::openLink, this, &BasicView::openLink);
    connect(addonsLinkButton, &LinkButton::openLink, this, &BasicView::openLink);
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
        return "Restart and reconfigure the running cluster";
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
    topStatusButton->setText(cluster.status());
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
    QCheckBox warnCloseCheck(&dialog);
    binaryPath.setText(m_setting.minikubeBinaryPath());
    warnCloseCheck.setChecked(m_setting.skipWarningOnClose());

    form.addRow(new QLabel(tr("Path to minikube binary")), &binaryPath);
    form.addRow(new QLabel(tr("Skip warn runs in background on close")), &warnCloseCheck);

    buttonBox.addButton(QString(tr("Save")), QDialogButtonBox::AcceptRole);
    buttonBox.addButton(QString(tr("Cancel")), QDialogButtonBox::RejectRole);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    int code = dialog.exec();
    if (code != QDialog::Accepted) {
        return;
    }
    Setting *s = new Setting();
    s->setMinikubeBinaryPath(binaryPath.text());
    s->setSkipWarningOnClose(warnCloseCheck.isChecked());
    m_setting = *s;
    emit saveSettings(*s);
}

void BasicView::receivedSettings(Setting s)
{
    m_setting = s;
}

void BasicView::minikubeNotFound()
{
    QDialog dialog;
    dialog.setWindowTitle("minikube");
    dialog.setWindowIcon(m_icon);
    dialog.setModal(true);
    QFormLayout form(&dialog);
    QLabel *message = new QLabel();
    message->setText("minikube was not found on the path.\nPlease follow the install instructions "
                     "below to install minikube first.\nIf you do have minikube installed set the "
                     "location in the settings.\n");
    form.addWidget(message);
    QLabel *link = new QLabel();
    link->setOpenExternalLinks(true);
    link->setText("<a "
                  "href='https://minikube.sigs.k8s.io/docs/start/'>https://minikube.sigs.k8s.io/"
                  "docs/start/</a>");
    form.addWidget(link);
    QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
    buttonBox.addButton(QString(tr("OK")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    form.addRow(&buttonBox);
    dialog.exec();
}

void BasicView::displayAbout(){
    QDialog aboutDialog;
    aboutDialog.setWindowTitle("About Minikube GUI");
    aboutDialog.setFixedSize(300, 200);
    QVBoxLayout mainLayout(&aboutDialog);

    // iconLayout displays minikube icon
    QHBoxLayout iconLayout;
    iconLayout.setAlignment(Qt::AlignCenter);

    QLabel iconLabel;
    iconLabel.setPixmap(m_icon.pixmap(50, 50));
    iconLayout.addWidget(&iconLabel);
    mainLayout.addLayout(&iconLayout);

    // introLayout displays minikube gui info, home URL, and license URL
    QVBoxLayout introLayout;
    introLayout.setContentsMargins(10, 10, 10, 10);
    introLayout.setAlignment(Qt::AlignCenter);

    QLabel introLabel;
    introLabel.setText("A graphical interface for minikube.");
    introLayout.addWidget(&introLabel);

    // hyperlinkItemLayout
    QHBoxLayout hyperlinkItemLayout;
    hyperlinkItemLayout.setAlignment(Qt::AlignCenter);

    QLabel githubURLLabel;
    githubURLLabel.setText("<a href='https://github.com/kubernetes-sigs/minikube-gui'>GitHub</a>");
    githubURLLabel.setOpenExternalLinks(true);
    hyperlinkItemLayout.addWidget(&githubURLLabel);

    QLabel licenseURLLabel;
    licenseURLLabel.setText("<a href='https://raw.githubusercontent.com/kubernetes-sigs/minikube-gui/main/LICENSE'>License</a>");
    licenseURLLabel.setOpenExternalLinks(true);
    hyperlinkItemLayout.addWidget(&licenseURLLabel);

    introLayout.addLayout(&hyperlinkItemLayout);
    mainLayout.addLayout(&introLayout);

    // versionItemLayout displays minikube gui version
    // TODO: display minikube binary version
    QHBoxLayout versionItemLayout;
    versionItemLayout.setAlignment(Qt::AlignCenter);

    QLabel versionLabel("Version: " + m_version.toString());
    versionLabel.setStyleSheet("QLabel { color: gray; }");
    versionLabel.setAlignment(Qt::AlignCenter);
    versionLabel.setTextInteractionFlags(Qt::TextSelectableByMouse);
    versionItemLayout.addWidget(&versionLabel);
    mainLayout.addLayout(&versionItemLayout);

    aboutDialog.exec();
}

