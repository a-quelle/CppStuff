#include "clickableqlabel.h"

ClickableQLabel::ClickableQLabel(QWidget* parent, Qt::WindowFlags f):
    QLabel(parent, f)
{}
ClickableQLabel::ClickableQLabel(const QString& text, QWidget* parent, Qt::WindowFlags f):
    QLabel(text, parent, f)
{}

void ClickableQLabel::mousePressEvent(QMouseEvent *ev){
    emit mouseClickSignal(ev);
}

ClickableQLabel::~ClickableQLabel(){}
