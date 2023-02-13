#ifndef SERVICEVIEW_H
#define SERVICEVIEW_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QDialog>

class ServiceView : public QObject
{
    Q_OBJECT

public:
    explicit ServiceView(QDialog *parent, QIcon icon);

    void displayTable(QString);

private:
    QDialog *m_dialog;
    QIcon m_icon;
    QDialog *m_parent;
};

#endif // SERVICEVIEW_H
