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

#include "updater.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QEventLoop>
#include <QFormLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QProcess>

Updater::Updater(QWidget *parent, QVersionNumber version, QIcon icon)
{
    m_version = version;
    m_icon = icon;
    m_parent = parent;
}

static bool checkedForUpdateRecently()
{
    QString filePath = QStandardPaths::locate(QStandardPaths::HomeLocation,
                                              "/.minikube-gui/last_update_check");
    if (filePath == "") {
        return false;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    QDateTime nextCheck = QDateTime::fromString(line).addSecs(60 * 60 * 24);
    QDateTime now = QDateTime::currentDateTime();
    return nextCheck > now;
}

static void logUpdateCheck()
{
    QDir dir = QDir(QDir::homePath() + "/.minikube-gui");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString filePath = dir.filePath("last_update_check");
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    QTextStream stream(&file);
    stream << QDateTime::currentDateTime().toString() << "\n";
}

void Updater::checkForUpdates()
{
    if (checkedForUpdateRecently()) {
        return;
    }
    logUpdateCheck();
    QString releases = getRequest("https://storage.googleapis.com/minikube-gui/releases.json");
    QJsonObject latestRelease =
            QJsonDocument::fromJson(releases.toUtf8()).array().first().toObject();
    QString latestReleaseVersion = latestRelease["name"].toString();
    QVersionNumber latestReleaseVersionNumber = QVersionNumber::fromString(latestReleaseVersion);
    if (m_version >= latestReleaseVersionNumber) {
        return;
    }
    QJsonObject links = latestRelease["links"].toObject();
    QString key;
#if __linux__
    key = "linux";
#elif __APPLE__
    key = "darwin";
#else
    key = "windows";
#endif
    QString link = links[key].toString();
#if __APPLE__
    askToUpdate(latestReleaseVersion, link);
#else
    notifyUpdate(latestReleaseVersion, link);
#endif
}

void Updater::askToUpdate(QString latest, QString link)
{
    QDialog dialog;
    dialog.setWindowTitle(tr("minikube GUI Update Available"));
    dialog.setWindowIcon(m_icon);
    dialog.setModal(true);
    QFormLayout form(&dialog);
    QLabel *msgLabel = new QLabel();
    msgLabel->setText("Version " + latest
                      + " of minikube GUI is now available!\n\nWould you like to update?");
    form.addWidget(msgLabel);
    QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
    buttonBox.addButton(QString(tr("Yes")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    buttonBox.addButton(QString(tr("No")), QDialogButtonBox::RejectRole);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow(&buttonBox);
    int code = dialog.exec();
    if (code == QDialog::Accepted) {
        downloadUpdate(link);
    }
}

void Updater::downloadUpdate(QString link)
{
    QDialog dialog(m_parent);
    dialog.setWindowTitle(tr("Downloading update"));
    dialog.setWindowIcon(m_icon);
    dialog.setModal(true);
    QFormLayout form(&dialog);
    QLabel *msgLabel = new QLabel("Downloading update, please wait...");
    form.addWidget(msgLabel);
    dialog.open();
    QProcess *process = new QProcess(this);
    QString cmd = QString("cd $TMPDIR && curl -LO %1 && tar -xf minikube-gui-macos.tar.gz && rm "
                          "minikube-gui-macos.tar.gz")
                          .arg(link);
    process->start("bash", { "-c", cmd });
    process->waitForFinished(-1);
    QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
    buttonBox.addButton(QString(tr("Restart")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    form.addRow(&buttonBox);
    msgLabel->setText(
            "Download complete!\n\nAfter clicking 'Restart' do the following:\n\n1. You will see: "
            "\"minikube-gui cannot be opened...\" click 'Cancel'\n2. Your Appliciations folder "
            "will be opened, right click on minikube-gui and click 'Open'");
    dialog.exec();
    process->start("bash",
                   { "-c",
                     "cd $TMPDIR && rm -rf /Applications/minikube-gui.app/ && mv "
                     "minikube-gui-macos/minikube-gui.app/ /Applications/ && rm -rf "
                     "minikube-gui-macos && open -R /Applications/minikube-gui.app/ && open "
                     "/Applications/minikube-gui.app/" });
    exit(0);
}

void Updater::notifyUpdate(QString latest, QString link)
{
    QDialog dialog;
    dialog.setWindowTitle(tr("minikube GUI Update Available"));
    dialog.setWindowIcon(m_icon);
    dialog.setModal(true);
    QFormLayout form(&dialog);
    QLabel *msgLabel = new QLabel();
    msgLabel->setText("Version " + latest
                      + " of minikube GUI is now available!\n\nDownload the update from:");
    form.addWidget(msgLabel);
    QLabel *linkLabel = new QLabel();
    linkLabel->setOpenExternalLinks(true);
    linkLabel->setText("<a href=\"" + link + "\">" + link + "</a>");
    form.addWidget(linkLabel);
    QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
    buttonBox.addButton(QString(tr("OK")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    form.addRow(&buttonBox);
    dialog.exec();
}

QString Updater::getRequest(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error()) {
            qDebug() << reply->errorString();
        }
    });
    QNetworkReply *resp = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    connect(resp, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return resp->readAll();
}
