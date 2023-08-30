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
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTableWidget>
#include <QDebug>
#include <QHeaderView>
#include <QStandardPaths>
#ifndef QT_NO_TERMWIDGET
#include "qtermwidget.h"
#include <QApplication>
#endif

ServiceView::ServiceView(QDialog *parent, QIcon icon, CommandRunner *runner, Settings *settings)
{
    m_parent = parent;
    m_icon = icon;
    m_commandRunner = runner;
    m_settings = settings;
}

void ServiceView::displayTable(QString svcCmdOutput)
{
    m_dialog = new QDialog(m_parent);
    m_dialog->setWindowFlags(m_dialog->windowFlags());
    m_dialog->setWindowTitle(tr("Service List"));
    m_dialog->setWindowIcon(m_icon);
    m_dialog->setMinimumWidth(800);
    m_dialog->setModal(true);
    QBoxLayout layout(QBoxLayout::TopToBottom, m_dialog);
    QJsonParseError error;
    QJsonDocument json = QJsonDocument::fromJson(svcCmdOutput.toUtf8(), &error);
    if (json.isNull() || !json.isArray()) {
        // cannot deseralize the output, print it to the dialog
        qDebug() << error.errorString();
        QLabel *tableLbl = new QLabel();
        tableLbl->setOpenExternalLinks(true);
        tableLbl->setWordWrap(true);
        tableLbl->setText(svcCmdOutput);
        layout.addWidget(tableLbl);
    } else {
        // can deseralize the output, generate a table
        QJsonArray serviceArray = json.array();
        QTableWidget *table = new QTableWidget(serviceArray.size(), 5);
        table->resizeColumnsToContents();
        QStringList headers({ "Namespace", "Name", "Target Port", "URL", "Actions" });
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->horizontalHeader()->setStretchLastSection(true);
        table->setHorizontalHeaderLabels(headers);
        for (int i = 0; i < serviceArray.size(); i++) {
            QJsonObject obj = serviceArray[i].toObject();
            QString nameSpace, serviceName;
            if (obj.contains("Namespace")) {
                nameSpace = obj["Namespace"].toString();
                QTableWidgetItem *namespaceItem = new QTableWidgetItem(nameSpace);
                table->setItem(i, 0, namespaceItem);
            }
            if (obj.contains("Name")) {
                serviceName = obj["Name"].toString();
                QTableWidgetItem *nameItem = new QTableWidgetItem(serviceName);
                table->setItem(i, 1, nameItem);
            }
            if (obj.contains("PortNames")) {
                QString listString =
                        QJsonDocument(obj["PortNames"].toArray()).toJson(QJsonDocument::Compact);
                QTableWidgetItem *portNamesItem = new QTableWidgetItem(listString);
                table->setItem(i, 2, portNamesItem);
            }
            if (obj.contains("URLs")) {
                QString listString =
                        QJsonDocument(obj["URLs"].toArray()).toJson(QJsonDocument::Compact);
                QTableWidgetItem *URLItem = new QTableWidgetItem(listString);
                table->setItem(i, 3, URLItem);
            }
            QPushButton *button = new QPushButton();
            button->setText("Connect");
            table->setCellWidget(i, 4, button);
            connect(button, &QPushButton::clicked, this, [this, serviceName, nameSpace]() -> void {
                qDebug() << "here";
                this->runMinikubeService(nameSpace, serviceName);
            });
        }
        layout.addWidget(table);
    }

    QDialogButtonBox buttonBox(Qt::Horizontal);
    buttonBox.addButton(QString(tr("OK")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, m_dialog, &QDialog::accept);
    layout.addWidget(&buttonBox);
    m_dialog->exec();
}

void ServiceView::runMinikubeService(QString nameSpace, QString serviceName)
{
    QStringList commandArgs = { "service", "-n", nameSpace, serviceName };
    QString command = m_settings->minikubePath() + " " + commandArgs.join(" ");

#ifndef QT_NO_TERMWIDGET
    int startnow = 0; // set shell program first

    QTermWidget *console = new QTermWidget(startnow);

    QFont font = QApplication::font();
    font.setFamily("Monospace");
    font.setPointSize(10);

    console->setTerminalFont(font);
    console->setColorScheme("Tango");
    console->setShellProgram("eval");
    console->setArgs({ commandArgs });
    console->startShellProgram();

#elif __APPLE__
    QStringList arguments = { "-e", "tell app \"Terminal\"",
                              "-e", "do script \"" + command + "\"",
                              "-e", "activate",
                              "-e", "end tell" };
    m_commandRunner->executeCommand("/usr/bin/osascript", arguments);
#else
    QString terminal = qEnvironmentVariable("TERMINAL");
    if (terminal.isEmpty()) {
        terminal = "x-terminal-emulator";
        if (QStandardPaths::findExecutable(terminal).isEmpty()) {
            terminal = "xterm";
        }
    }

    m_commandRunner->executeCommand(QStandardPaths::findExecutable(terminal), { "-e", command });
#endif
}
