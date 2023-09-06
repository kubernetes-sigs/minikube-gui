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

#ifndef LINKBUTTON_H
#define LINKBUTTON_H

#include <QPushButton>
#include <QSize>
#include <QEnterEvent>

class LinkButton: public QPushButton
{
    Q_OBJECT

public:
    explicit LinkButton(const QString &icon, const QString &link, QWidget *parent = nullptr);
    QSize sizeHint() const override;
       
signals:
    void openLink(const QString &link);

protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent *event) override;
#else
    void enterEvent(QEvent *event) override;
#endif
    void leaveEvent(QEvent *event) override;

private slots:
    void emitOpenLink();

private:
    QString m_link;
};

#endif // PUSHBUTTONWITHLINK_H
