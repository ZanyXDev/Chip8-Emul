#include "screen.h"


Screen::Screen( QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); // set ideal size as minimum Qimage size/ Can expand, only
    zoom = 8; // TODO add ability change ixel image size default 8x8

    m_display = QBitArray(DISPLAY_X * DISPLAY_Y,false);

    bgColor = Qt::darkGray;
    fgColor = Qt::black;
    lineColor = Qt::gray;

    m_pixmap = new QPixmap ( sizeHint() );
}

QSize Screen::sizeHint() const
{
    QSize size = QSize(zoom * DISPLAY_X, zoom * DISPLAY_Y);
    if (zoom >= 3)
    {
        // add space for coordinate line show
        size += QSize(1, 1);
    }
    return size;
}

void Screen::updateScreen(QBitArray display)
{   
    m_display = display;
    update();
}

void Screen::drawOnSurface()
{
    QPainter painter(m_pixmap);
    if (zoom >= 3)
    {
        QPen pen(Qt::gray, 0, Qt::SolidLine);
        painter.setPen(pen);

        for (int x = 0; x <=DISPLAY_X; ++x)
        {
            painter.drawLine(zoom * x, 0, zoom * x, zoom * DISPLAY_Y);
        }
        for (int y = 0; y <=DISPLAY_Y; ++y)
        {
            painter.drawLine(0, zoom * y,  zoom * DISPLAY_X, zoom * y);
        }
    }
    for (int x = 0; x < DISPLAY_X; ++x)
    {
        for (int y = 0; y < DISPLAY_Y; ++y)
        {
            drawImagePixel(&painter, x, y);
        }
    }
}

void Screen::paintEvent(QPaintEvent *event)
{
    drawOnSurface();

    QPainter painter (this);
    painter.fillRect(event->rect(),Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //FIXME dirty hack :( drawPixmap target equ source. if not view artefact
    painter.drawPixmap(event->rect(), m_pixmap->scaled( size() , Qt::KeepAspectRatio),event->rect());
}

void Screen::drawImagePixel(QPainter *painter, int x, int y)
{
    QColor color;
    quint16 val = x + (y * DISPLAY_X );
    quint16 idx = ( val > MAX_DISPLAY_SIZE ) ? MAX_DISPLAY_SIZE : val;

    if (m_display.at(idx))
    {  //TODO add ability change color from menu
        color = Qt::black;
    }
    else
    {
        color = Qt::white;
    }

    if (zoom >= 3)
    {
        painter->fillRect(zoom * x + 1, zoom * y + 1, zoom - 1, zoom - 1, color);
    }
    else
    {
        painter->fillRect(zoom * x, zoom * y,zoom, zoom, color);
    }

}
