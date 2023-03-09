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

#include "serviceview.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>

ServiceView::ServiceView(QDialog *parent, QIcon icon)
{
    m_parent = parent;
    m_icon = icon;
}

void ServiceView::displayTable(QString svcCmdOutput)
{
    m_dialog = new QDialog(m_parent);
    m_dialog->setWindowTitle(tr("Service List"));
    m_dialog->setWindowIcon(m_icon);
    m_dialog->setFixedWidth(600);
    m_dialog->setModal(true);
    QFormLayout form(m_dialog);
    QLabel *tableLbl = new QLabel();
    tableLbl->setOpenExternalLinks(true);
    tableLbl->setWordWrap(true);
    tableLbl->setText(svcCmdOutput);
    form.addRow(tableLbl);
    QDialogButtonBox buttonBox(Qt::Horizontal, m_dialog);
    buttonBox.addButton(QString(tr("OK")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, m_dialog, &QDialog::accept);
    form.addRow(&buttonBox);
    m_dialog->exec();
}
