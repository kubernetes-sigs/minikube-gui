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

#include "addonsview.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QDialog>

AddonsView::AddonsView(QIcon icon)
{
    m_icon = icon;
    m_parent = 0;
    m_dialog = new QDialog(m_parent);
    m_dialog->setWindowTitle(tr("Addons List"));
    m_dialog->setWindowIcon(m_icon);
    m_dialog->setFixedWidth(600);
    m_dialog->setModal(true);

    AddonList addons;
    m_addonModel = new AddonModel(addons);

    addonListView = new QTableView();
    addonListView->setModel(m_addonModel);
    addonListView->setSelectionMode(QAbstractItemView::NoSelection);
    addonListView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    addonListView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    addonListView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    refreshButton = new QPushButton(tr("Refresh"));
    QVBoxLayout *bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget(refreshButton);
    QVBoxLayout *addonsLayout = new QVBoxLayout;
    addonsLayout->addWidget(addonListView);
    addonsLayout->addLayout(bottomLayout);
    m_dialog->setLayout(addonsLayout);

    connect(refreshButton, &QAbstractButton::clicked, this, &AddonsView::refresh);
}

void AddonsView::updateAddonsTable(AddonList addonList)
{
    m_addonModel->setAddonList(addonList);
    buttonList = new QList<QPushButton>();
    for (int i = 0; i < addonList.size(); i++) {
        Addon addon = addonList[i];
        QString addonName = addon.name();
        QString action = addon.status() == "enabled" ? "disable" : "enable";
        QPushButton *button = new QPushButton();
        connect(button, &QPushButton::clicked,
                [this, addonName, action] { emit addonClicked(addonName, action); });
        addonListView->setIndexWidget(addonListView->model()->index(i, 2), button);
    }
}

void AddonsView::display()
{
    m_dialog->exec();
}
