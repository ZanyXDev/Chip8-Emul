#include "display.h"
#include <QPainter>
#include <QDebug>
#include <QImage>
#include <QRect>

Display::Display(QWidget *parent)
    : QWidget (parent)
{   
    antialiased = false;
    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->debugDraw();
}


void Display::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void Display::paintEvent(QPaintEvent *event)
{
    QRect  m_rect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, antialiased);
    painter.fillRect(m_rect, QBrush(Qt::gray));
    int size_x = m_rect.width() / 64;
    int size_y = m_rect.height() / 32;

    painter.setPen(Qt::green);
    painter.setBrush(Qt::SolidPattern);
    painter.drawRect(0, 0, size_x, size_y);
}

void Display::resizeEvent(QResizeEvent *event)
{
    /*
    if (width() > m_screenImage.width() || height() > m_screenImage.height()) {
        int newWidth = qMax(width() + 128, m_screenImage.width());
        int newHeight = qMax(height() + 128, m_screenImage.height());
        resizeImage(&m_screenImage, QSize(newWidth, newHeight));
        update();
    }
    */
    QWidget::resizeEvent(event);
}

void Display::debugDraw()
{
    update();
}



QSize Display::minimumSizeHint() const
{
    return QSize(64, 32);
}

QSize Display::sizeHint() const
{
    return QSize(320, 160);
}

