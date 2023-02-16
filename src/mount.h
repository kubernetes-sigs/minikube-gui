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

#ifndef MOUNT_H
#define MOUNT_H

#include <QProcess>

class Mount
{
public:
    Mount(QString src, QString dest, QProcess *proc) : m_src(src), m_dest(dest), m_proc(proc) { }

    QString name() const { return m_src + ":" + m_dest; }
    QString src() const { return m_src; }
    QString dest() const { return m_dest; }
    QString logs() const { return m_logs; }
    void setLogs(QString logs) { m_logs = logs; }
    QProcess *proc() const { return m_proc; }

private:
    QString m_src;
    QString m_dest;
    QProcess *m_proc;
    QString m_logs;
};

typedef QList<Mount> MountList;
typedef QHash<QString, Mount> MountHash;

#endif // MOUNT_H
