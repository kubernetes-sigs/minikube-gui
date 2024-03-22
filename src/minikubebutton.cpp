#include "minikubebutton.h"

MinikubeButton::MinikubeButton(QWidget *parent) : QPushButton(parent)
{
    // By default a button is enabled when instantiated
    setStyleSheet(enabledStyleSheet);
}

MinikubeButton::MinikubeButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{
    setStyleSheet(enabledStyleSheet);
}

MinikubeButton::MinikubeButton(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{
    setStyleSheet(enabledStyleSheet);
}

void MinikubeButton::setEnabled(bool enabled)
{
    QPushButton::setEnabled(enabled);
    setStyleSheet(enabled ? enabledStyleSheet : diabledStyleSheet);
}

bool MinikubeButton::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::HoverEnter:
        if (isEnabled()) {
            setStyleSheet(enabledHoverStyleSheet);
        }
        break;
    case QEvent::HoverLeave:
        if (isEnabled()) {
            setStyleSheet(enabledStyleSheet);
        }

    default:
        break;
    }
    return QPushButton::event(event);
}
