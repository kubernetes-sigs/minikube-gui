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


#ifndef SERVICEVIEW_H
#define SERVICEVIEW_H

#include <QObject>
#include <QIcon>
#include <QString>
#include <QDialog>

class ServiceView : public QObject
{
    Q_OBJECT

public:
    explicit ServiceView(QDialog *parent, QIcon icon);

    void displayTable(QString);

private:
    QDialog *m_dialog;
    QIcon m_icon;
    QDialog *m_parent;
};

#endif // SERVICEVIEW_H
