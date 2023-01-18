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

#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QDialog>
#include <QFormLayout>
#include <QIcon>
#include <QLabel>
#include <QObject>

class ErrorMessage : public QObject
{
    Q_OBJECT

public:
    explicit ErrorMessage(QDialog *parent, QIcon icon);

    void error(QString errorCode, QString advice, QString errorMessage, QString url,
               QString issues);
    QLabel *createLabel(QString title, QString text, QFormLayout *form, bool isLink);

private:
    QDialog *m_dialog;
    QIcon m_icon;
    QDialog *m_parent;
};

#endif // ERRORMESSAGE_H
