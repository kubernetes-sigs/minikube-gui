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
#include "fonts.h"
#include "constants.h"
#include "serviceview.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QCloseEvent>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QStandardPaths>

#ifndef QT_NO_TERMWIDGET
#include "qtermwidget.h"
#include <QApplication>
#include <QMainWindow>
#endif

const QVersionNumber version = QVersionNumber::fromString("0.2.0");

Window::Window()
{
    trayIconIcon = new QIcon(":/resources/images/minikube.png");
    Fonts::initFonts();

    stackedWidget = new QStackedWidget;
    logger = new Logger();
    settings = new Settings();
    commandRunner = new CommandRunner(this, logger, settings);
    basicView = new BasicView(*trayIconIcon);
    serviceView = new ServiceView(this, *trayIconIcon);
    addonsView = new AddonsView(*trayIconIcon);
    advancedView = new AdvancedView(*trayIconIcon);
    errorMessage = new ErrorMessage(this, *trayIconIcon);
    progressWindow = new ProgressWindow(this, *trayIconIcon);
    tray = new Tray(*trayIconIcon);
    hyperKit = new HyperKit(*trayIconIcon);
    updater = new Updater(this, version, *trayIconIcon);

    op = new Operator(advancedView, basicView, serviceView, addonsView, commandRunner, errorMessage,
                      progressWindow, tray, hyperKit, updater, settings, stackedWidget, this);

    stackedWidget->addWidget(basicView->basicView);
    stackedWidget->addWidget(advancedView->advancedView);
    layout = new QVBoxLayout;
    layout->addWidget(stackedWidget);
    setLayout(layout);
    resize(Constants::basicViewWidth, Constants::basicViewHeight);
    setWindowTitle("minikube");
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
    bool skipWarning = settings->getSettings().skipWarningOnClose();
    if (QSystemTrayIcon::isSystemTrayAvailable() && tray->isVisible() && !skipWarning) {
        QMessageBox::information(this, "minikube",
                                 tr("minikube will minimize to the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

#endif
