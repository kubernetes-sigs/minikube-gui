/*
Copyright 2023 The Kubernetes Authors All rights reserved.

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

#include "settings.h"
#include "paths.h"

#include <QSettings>
#include <QDir>

Settings::Settings()
{
    QDir dir = QDir(QDir::homePath() + "/.minikube-gui");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    m_configPath = dir.filePath("config.ini");
    QSettings settings(m_configPath, QSettings::IniFormat);
    if (settings.contains("minikube-binary-path")) {
        return;
    }
    settings.setValue("minikube-binary-path", Paths::minikubePath());
}

void Settings::updateSettings(Setting s)
{
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.setValue("minikube-binary-path", s.minikubeBinaryPath());
    settings.setValue("skip-warning-on-close", s.skipWarningOnClose());
}

Setting Settings::getSettings()
{
    QSettings *s = new QSettings(m_configPath, QSettings::IniFormat);
    Setting *setting = new Setting();
    setting->setMinikubeBinaryPath(
            s->value("minikube-binary-path", Paths::minikubePath()).toString());
    setting->setSkipWarningOnClose(s->value("skip-warning-on-close").toBool());
    return *setting;
}

QString Settings::minikubePath()
{
    return getSettings().minikubeBinaryPath();
}
