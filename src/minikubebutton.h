#ifndef MINIKUBEBUTTON_H
#define MINIKUBEBUTTON_H
#include <QPushButton>
#include <QEvent>
class MinikubeButton : public QPushButton
{
public:
    explicit MinikubeButton(QWidget *parent = nullptr);
    explicit MinikubeButton(const QString &text, QWidget *parent = nullptr);
    MinikubeButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    void setEnabled(bool enabled);

    virtual bool event(QEvent *event) override;

private:
    const QString enabledStyleSheet = "";
    const QString enabledHoverStyleSheet = "background-color:rgb(105,192,255,50)";

    const QString diabledStyleSheet =
            "background-color:rgb(191,191,191,50);color:rgb(140,140,140);";
};

#endif // MINIKUBEBUTTON_H
