#include "display.h"

Display::Display(QObject *parent) : QObject(parent)
{
    m_screen = QBitArray( DISPLAY_X * DISPLAY_Y, false );
    m_ExtendedMode = false;
}

void Display::clear()
{
    m_screen = QBitArray( DISPLAY_X * DISPLAY_Y, false );
}

void Display::moveDown(quint8 m_line)
{

}

void Display::moveRight()
{

}

void Display::moveLeft()
{

}

bool Display::drawPixel(quint16 idx, bool pixel)
{
    bool existPixel = false;

    if ( idx < m_screen.size() )
    {
        existPixel = m_screen.testBit( idx );
        m_screen.setBit( idx, (existPixel ^ pixel) );
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " Index (m_screen) out of range:"
                 << (idx) << " m_screen.size():" << m_screen.size();
    }

    return existPixel;
}

bool Display::getPixel(quint16 idx)
{
    //TODO check < or <=
    return ( idx <= m_screen.size() )
            ? m_screen.at( idx )
            : false;
}

void Display::setHiResMode(bool mode)
{
    m_ExtendedMode = mode;
}

bool Display::getHiResMode()
{
    return m_ExtendedMode;
}
