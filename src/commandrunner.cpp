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

#include "commandrunner.h"
#include "paths.h"

#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardPaths>

CommandRunner::CommandRunner(QDialog *parent, Logger *logger)
{
    m_env = QProcessEnvironment::systemEnvironment();
    m_parent = parent;
    m_logger = logger;
    minikubePath();
#if __APPLE__
    setMinikubePath();
#endif
}

void CommandRunner::executeCommand(QString program, QStringList args)
{
    QProcess *process = new QProcess(this);
    process->setProcessEnvironment(m_env);
    process->start(program, args);
    process->waitForFinished(-1);
    if (process->exitCode() == 0) {
        return;
    }
    QString out = process->readAllStandardOutput();
    QString err = process->readAllStandardError();
    QString log = QString("The following command failed:\n%1 "
                          "%2\n\nStdout:\n%3\n\nStderr:\n%4\n\n")
                          .arg(program, args.join(" "), out, err);
    m_logger->log(log);
    delete process;
}

void CommandRunner::executeMinikubeCommand(QStringList args)
{
    m_isRunning = true;
    m_output = "";
    QStringList userArgs = { "--user", "minikube-gui" };
    args << userArgs;
    m_process = new QProcess(m_parent);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &CommandRunner::executionCompleted);
    connect(m_process, &QProcess::readyReadStandardError, this, &CommandRunner::errorReady);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &CommandRunner::outputReady);
    m_process->setProcessEnvironment(m_env);
    m_process->start(m_minikubePath, args);
    emit CommandRunner::startingExecution();
}

void CommandRunner::executeMinikubeCommand(QStringList args, QProcess *process)
{
    QStringList userArgs = { "--user", "minikube-gui" };
    args << userArgs;
    process->setProcessEnvironment(m_env);
    process->start(m_minikubePath, args);
}

void CommandRunner::startMinikube(QStringList args)
{
    m_command = "start";
    QStringList baseArgs = { "start", "-o", "json" };
    baseArgs << args;
    m_args = baseArgs;
    executeMinikubeCommand(baseArgs);
    emit startCommandStarting();
}

void CommandRunner::stopMinikube(QStringList args)
{
    QStringList baseArgs = { "stop" };
    baseArgs << args;
    executeMinikubeCommand(baseArgs);
}

void CommandRunner::pauseMinikube(QStringList args)
{
    QStringList baseArgs = { "pause" };
    baseArgs << args;
    executeMinikubeCommand(baseArgs);
}

void CommandRunner::unpauseMinikube(QStringList args)
{
    QStringList baseArgs = { "unpause" };
    baseArgs << args;
    executeMinikubeCommand(baseArgs);
}

void CommandRunner::deleteMinikube(QStringList args)
{
    m_command = "delete";
    QStringList baseArgs = { "delete" };
    baseArgs << args;
    executeMinikubeCommand(baseArgs);
}

void CommandRunner::mountMinikube(QStringList args, QProcess *process)
{
    QStringList baseArgs = { "mount" };
    baseArgs << args;
    executeMinikubeCommand(baseArgs, process);
}

void CommandRunner::tunnelMinikube(QStringList args, QProcess *process)
{
    QStringList baseArgs = { "tunnel" };
    baseArgs << args;
    baseArgs.prepend(m_minikubePath);
    QString command = baseArgs.join(" ");
#ifndef QT_NO_TERMWIDGET
    QMainWindow *mainWindow = new QMainWindow();
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

    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    mainWindow->setWindowTitle(nameLabel->text());
    mainWindow->resize(800, 400);
    mainWindow->setCentralWidget(console);
    mainWindow->show();
#elif __APPLE__
    QStringList arguments = { "-e", "tell app \"Terminal\"",
                              "-e", "do script \"" + command + "\"",
                              "-e", "activate",
                              "-e", "end tell" };
    executeCommand("/usr/bin/osascript", arguments);
#else
    QString terminal = qEnvironmentVariable("TERMINAL");
    if (terminal.isEmpty()) {
        terminal = "x-terminal-emulator";
        if (QStandardPaths::findExecutable(terminal).isEmpty()) {
            terminal = "xterm";
        }
    }

    executeCommand(QStandardPaths::findExecutable(terminal), { "-e", command });
#endif
}

void CommandRunner::dashboardMinikube(QStringList args, QProcess *process)
{
    QStringList baseArgs = { "dashboard" };
    baseArgs << args;
    executeMinikubeCommand(baseArgs, process);
}

void CommandRunner::stopCommand()
{
    m_process->terminate();
}

static Cluster createClusterObject(QJsonObject obj)
{
    QString name;
    if (obj.contains("Name")) {
        name = obj["Name"].toString();
    }
    Cluster cluster(name);
    if (obj.contains("Status")) {
        QString status = obj["Status"].toString();
        cluster.setStatus(status);
    }
    if (!obj.contains("Config")) {
        return cluster;
    }
    QJsonObject config = obj["Config"].toObject();
    if (config.contains("CPUs")) {
        int cpus = config["CPUs"].toInt();
        cluster.setCpus(cpus);
    }
    if (config.contains("Memory")) {
        int memory = config["Memory"].toInt();
        cluster.setMemory(memory);
    }
    if (config.contains("Driver")) {
        QString driver = config["Driver"].toString();
        cluster.setDriver(driver);
    }
    if (!config.contains("KubernetesConfig")) {
        return cluster;
    }
    QJsonObject k8sConfig = config["KubernetesConfig"].toObject();
    if (k8sConfig.contains("ContainerRuntime")) {
        QString containerRuntime = k8sConfig["ContainerRuntime"].toString();
        cluster.setContainerRuntime(containerRuntime);
    }
    if (k8sConfig.contains("KubernetesVersion")) {
        QString k8sVersion = k8sConfig["KubernetesVersion"].toString();
        cluster.setK8sVersion(k8sVersion);
    }
    return cluster;
}

static ClusterList jsonToClusterList(QString text)
{
    ClusterList clusters;
    QStringList lines;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    lines = text.split("\n", Qt::SkipEmptyParts);
#else
    lines = text.split("\n", QString::SkipEmptyParts);
#endif
    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(line.toUtf8(), &error);
        if (json.isNull()) {
            qDebug() << error.errorString();
            continue;
        }
        if (!json.isObject()) {
            continue;
        }
        QJsonObject par = json.object();
        QJsonArray valid = par["valid"].toArray();
        QJsonArray invalid = par["invalid"].toArray();
        for (int i = 0; i < valid.size(); i++) {
            QJsonObject obj = valid[i].toObject();
            Cluster cluster = createClusterObject(obj);
            clusters << cluster;
        }
        for (int i = 0; i < invalid.size(); i++) {
            QJsonObject obj = invalid[i].toObject();
            Cluster cluster = createClusterObject(obj);
            cluster.setStatus("Invalid");
            clusters << cluster;
        }
    }
    return clusters;
}

void CommandRunner::requestClusters()
{
    m_command = "cluster";
    QStringList args = { "profile", "list", "-o", "json" };
    executeMinikubeCommand(args);
}

void CommandRunner::requestServiceList(QString pName)
{
    m_command = "service";
    QStringList args = {"-p", pName, "service", "list" };
    executeMinikubeCommand(args);
}

void CommandRunner::executionCompleted()
{
    m_isRunning = false;
    QString cmd = m_command;
    m_command = "";
    QString output = m_output;
    int exitCode = m_process->exitCode();
    delete m_process;
    if (cmd != "cluster") {
        emit executionEnded();
    }
    if (cmd == "start" && exitCode != 0) {
        emit error(m_args, output);
    }
    if (cmd == "cluster") {
        ClusterList clusterList = jsonToClusterList(output);
        emit updatedClusters(clusterList);
    }
    if (cmd == "service") {
        emit updatedServices(output);
    }
}

void CommandRunner::errorReady()
{
    QString text = m_process->readAllStandardError();
    m_output.append(text);
    emit output(text);
}

void CommandRunner::outputReady()
{
    QString text = m_process->readAllStandardOutput();
    m_output.append(text);
    emit output(text);
}

#if __APPLE__
void CommandRunner::setMinikubePath()
{
    m_env = QProcessEnvironment::systemEnvironment();
    QString path = m_env.value("PATH") + ":" + Paths::minikubePaths().join(":");
    m_env.insert("PATH", path);
}
#endif

void CommandRunner::minikubePath()
{
    m_minikubePath = QStandardPaths::findExecutable("minikube");
    if (!m_minikubePath.isEmpty()) {
        return;
    }
    m_minikubePath = QStandardPaths::findExecutable("minikube", Paths::minikubePaths());
}

bool CommandRunner::isRunning()
{
    return m_isRunning;
}
