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

#ifndef WINDOW_H
#define WINDOW_H

#include <QFormLayout>
#include <QProcessEnvironment>
#include <QProgressBar>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QVersionNumber>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QIcon;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
class QTableView;
class QProcess;
QT_END_NAMESPACE

#include "advancedview.h"
#include "basicview.h"
#include "serviceview.h"
#include "errormessage.h"
#include "hyperkit.h"
#include "logger.h"
#include "operator.h"
#include "progresswindow.h"
#include "tray.h"
#include "updater.h"

class Window : public QDialog
{
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible) override;
    void restoreWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QIcon *trayIconIcon;

    void checkForMinikube();
    QStackedWidget *stackedWidget;
    void checkForUpdates();
    QString getRequest(QString url);
    void notifyUpdate(QString latest, QString link);

    BasicView *basicView;
    ServiceView *serviceView;
    AdvancedView *advancedView;
    Operator *op;
    CommandRunner *commandRunner;
    ErrorMessage *errorMessage;
    ProgressWindow *progressWindow;
    Tray *tray;
    HyperKit *hyperKit;
    Updater *updater;
    QVBoxLayout *layout;
    Logger *logger;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif
