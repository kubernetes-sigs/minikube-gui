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

#include "progresswindow.h"

#include <QPushButton>
#include <QVBoxLayout>

ProgressWindow::ProgressWindow(QWidget *parent, QIcon icon)
{
    m_icon = icon;
    m_parent = parent;
    m_text = new QLabel();
}

void ProgressWindow::setBarMaximum(int max)
{
    m_progressBar->setMaximum(max);
}

void ProgressWindow::setBarValue(int value)
{
    m_progressBar->setValue(value);
}

void ProgressWindow::setText(QString text)
{
    m_text->setText(text);
}

void ProgressWindow::show()
{
    m_dialog = new QDialog(m_parent);
    m_dialog->setWindowIcon(m_icon);
    m_dialog->resize(300, 150);
    m_dialog->setWindowFlags(Qt::FramelessWindowHint);
    m_dialog->setModal(true);

    QVBoxLayout form(m_dialog);

    m_text->setWordWrap(true);
    form.addWidget(m_text);

    m_progressBar = new QProgressBar();
    form.addWidget(m_progressBar);

    m_cancelButton = new QPushButton();
    m_cancelButton->setText(tr("Cancel"));
    connect(m_cancelButton, &QAbstractButton::clicked, this, &ProgressWindow::cancel);
    form.addWidget(m_cancelButton);

    m_dialog->open();
}

void ProgressWindow::cancel()
{
    done();
    emit cancelled();
}

void ProgressWindow::done()
{
    m_dialog->hide();
    m_progressBar->setValue(0);
}
