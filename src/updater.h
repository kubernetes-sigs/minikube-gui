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

#ifndef UPDATER_H
#define UPDATER_H

#include <QIcon>
#include <QObject>
#include <QVersionNumber>

class Updater : public QObject
{
    Q_OBJECT

public:
    explicit Updater(QWidget *parent, QVersionNumber version, QIcon icon);
    void checkForUpdates();

private:
    void askToUpdate(QString latest, QString link);
    void downloadUpdate(QString link);
    void notifyUpdate(QString latest, QString link);
    QString getRequest(QString url);
    QVersionNumber m_version;
    QIcon m_icon;
    QWidget *m_parent;
};

#endif // UPDATER_H
