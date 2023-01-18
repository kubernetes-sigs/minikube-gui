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

#include "window.h"
#include "paths.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QCoreApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QFontDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTableView>
#include <QTextEdit>
#include <QVBoxLayout>

#ifndef QT_NO_TERMWIDGET
#include "qtermwidget.h"
#include <QApplication>
#include <QMainWindow>
#endif

const QVersionNumber version = QVersionNumber::fromString("0.0.1");

Window::Window()
{
    trayIconIcon = new QIcon(":/images/minikube.png");
    checkForMinikube();

    stackedWidget = new QStackedWidget;
    logger = new Logger();
    commandRunner = new CommandRunner(this, logger);
    basicView = new BasicView();
    advancedView = new AdvancedView(*trayIconIcon);
    errorMessage = new ErrorMessage(this, *trayIconIcon);
    progressWindow = new ProgressWindow(this, *trayIconIcon);
    tray = new Tray(*trayIconIcon);
    hyperKit = new HyperKit(*trayIconIcon);
    updater = new Updater(version, *trayIconIcon);

    op = new Operator(advancedView, basicView, commandRunner, errorMessage, progressWindow, tray,
                      hyperKit, updater, stackedWidget, this);

    stackedWidget->addWidget(basicView->basicView);
    stackedWidget->addWidget(advancedView->advancedView);
    layout = new QVBoxLayout;
    layout->addWidget(stackedWidget);
    setLayout(layout);
    resize(200, 300);
    setWindowTitle(tr("minikube"));
    setWindowIcon(*trayIconIcon);
}

void Window::setVisible(bool visible)
{
    tray->setVisible(visible);
    QDialog::setVisible(visible);
}

void Window::closeEvent(QCloseEvent *event)
{
#if __APPLE__
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (tray->isVisible()) {
        QMessageBox::information(this, tr("minikube"),
                                 tr("minikube will minimize to the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

static QString minikubePath()
{
    QString minikubePath = QStandardPaths::findExecutable("minikube");
    if (!minikubePath.isEmpty()) {
        return minikubePath;
    }
    return QStandardPaths::findExecutable("minikube", Paths::minikubePaths());
}

void Window::checkForMinikube()
{
    QString program = minikubePath();
    if (!program.isEmpty()) {
        return;
    }

    QDialog dialog;
    dialog.setWindowTitle(tr("minikube"));
    dialog.setWindowIcon(*trayIconIcon);
    dialog.setModal(true);
    QFormLayout form(&dialog);
    QLabel *message = new QLabel(this);
    message->setText("minikube was not found on the path.\nPlease follow the install instructions "
                     "below to install minikube first.\n");
    form.addWidget(message);
    QLabel *link = new QLabel(this);
    link->setOpenExternalLinks(true);
    link->setText("<a "
                  "href='https://minikube.sigs.k8s.io/docs/start/'>https://minikube.sigs.k8s.io/"
                  "docs/start/</a>");
    form.addWidget(link);
    QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
    buttonBox.addButton(QString(tr("OK")), QDialogButtonBox::AcceptRole);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    form.addRow(&buttonBox);
    dialog.exec();
    exit(EXIT_FAILURE);
}

#endif
