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

#ifndef ADDONSVIEW_H
#define ADDONSVIEW_H

#include "addon.h"

#include <QPushButton>
#include <QTableView>

class AddonsView : public QObject
{
    Q_OBJECT

public:
    explicit AddonsView(QIcon icon);
    QTableView *addonListView;
    void update(Addon addon);
    void updateAddonsTable(AddonList addonList);
    void display();
signals:
    void refresh();
    void addonClicked(QString addonName, QString action);

private:
    QIcon m_icon;
    AddonModel *m_addonModel;
    QPushButton *refreshButton;
    QDialog *m_dialog;
    QDialog *m_parent;
    QList<QPushButton> *buttonList;
};

#endif // ADDONSVIEW_H
