#include "display.h"
#include <QPainter>
#include <QBrush>
#include <QDebug>

Display::Display(QWidget *parent) : QWidget(parent)
{
    bgColor = Qt::black;
    fgColor = Qt::white;

    m_screenImage = new QImage(128,64,QImage::Format_Grayscale8);
}

void Display::updateDisplay(QVector <unsigned char> screen)
{

    QPainter painter(m_screenImage);
    painter.setRenderHint(QPainter::Qt4CompatiblePainting, true);
    //painter.scale ( resolution, resolution );
    QBrush brush(bgColor, Qt::SolidPattern);
    // TODO fix magic number
    painter.fillRect(0, 0, 128, 64, brush);
    painter.setPen(fgColor);
    painter.setBrush(fgColor);

    for (int y = 0; y < 64; y++){
        for (int x = 0; x < 128; x++){
            qDebug() << "y=" << y << " x=" << x;
            if ( screen.at(getIndex(x,y)) == 1)
            {
                //if (resolution > 1)
                //    painter.drawRect ( x, y, 1, 1 );
                //else painter.drawPoint(x, y);

                painter.drawPoint(x,y);
            }
        }
    }
    this->update();
}

void Display::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(4,4);
    painter.drawImage(0,0,*m_screenImage);
}

int Display::getIndex(int x, int y)
{
   // qDebug() << " index=" << x+(y*64);
    // TODO fix magic number
     return x+(y*64);
}
