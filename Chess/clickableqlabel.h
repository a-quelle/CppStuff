#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QLabel>
#include<QMessageBox>
#include<QMouseEvent>

class ClickableQLabel: public QLabel
{
    Q_OBJECT
public:
    ClickableQLabel(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
    ClickableQLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
    ~ClickableQLabel() override;
protected:
    void mousePressEvent(QMouseEvent* ev) override;
signals:
    void mouseClickSignal(QMouseEvent* ev);
};

#endif // CLICKABLEQLABEL_H
