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

#include "hyperkit.h"

#include <QMessageBox>
#include <QProcess>

HyperKit::HyperKit(QIcon icon)
{
    m_icon = icon;
}

#if __APPLE__
bool HyperKit::hyperkitPermissionFix(QStringList args, QString text)
{
    if (!text.contains("docker-machine-driver-hyperkit needs to run with elevated permissions")) {
        return false;
    }
    if (!showHyperKitMessage()) {
        return false;
    }

    hyperkitPermission();
    emit rerun(args);
    return true;
}

void HyperKit::hyperkitPermission()
{
    QString command = "sudo chown root:wheel ~/.minikube/bin/docker-machine-driver-hyperkit && "
                      "sudo chmod u+s ~/.minikube/bin/docker-machine-driver-hyperkit && exit";
    QStringList arguments = { "-e", "tell app \"Terminal\"",
                              "-e", "set w to do script \"" + command + "\"",
                              "-e", "activate",
                              "-e", "repeat",
                              "-e", "delay 0.1",
                              "-e", "if not busy of w then exit repeat",
                              "-e", "end repeat",
                              "-e", "end tell" };
    QProcess *process = new QProcess();
    process->start("/usr/bin/osascript", arguments);
    process->waitForFinished(-1);
}

bool HyperKit::showHyperKitMessage()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("HyperKit Permissions Required");
    msgBox.setWindowIcon(m_icon);
    msgBox.setModal(true);
    msgBox.setText("The HyperKit driver requires a one-time sudo permission.\n\nIf you'd like "
                   "to proceed, press OK and then enter your password into the terminal prompt, "
                   "the start will resume after.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int code = msgBox.exec();
    return code == QMessageBox::Ok;
}
#endif
