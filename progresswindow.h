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

#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QProcess>
#include <QProgressBar>
#include <QString>
#include <QIcon>

class ProgressWindow : public QObject
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent, QIcon icon);

    void setBarMaximum(int max);
    void setBarValue(int value);
    void setText(QString text);
    void show();
    void done();

signals:
    void cancelled();

private:
    void cancel();

    QDialog *m_dialog;
    QLabel *m_text;
    QProgressBar *m_progressBar;
    QPushButton *m_cancelButton;
    QIcon m_icon;
};

#endif // PROGRESSWINDOW_H
