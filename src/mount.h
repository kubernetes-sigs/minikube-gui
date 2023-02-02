#ifndef MOUNT_H
#define MOUNT_H
#include <QString>
#include <QProcess>
class Mount
{
public:
    Mount(QString src, QString dest, QProcess *proc)
        : m_src(src),m_dest(dest), m_proc(proc)
    {
    }

    QString name() const { return m_src+":"+m_dest; }
    QString src() const { return m_src; }
    QString dest() const { return m_dest; }
    QProcess *proc() const { return m_proc; }

private:
    QString m_src;
    QString m_dest;
    QProcess *m_proc;
    QString m_log;


};

typedef QList<Mount> MountList;
typedef QHash<QString, Mount> MountHash;

#endif // MOUNT_H
