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
    QBitArray m_buf = QBitArray( DISPLAY_X * DISPLAY_Y, false );
    quint16 idx;
    for (quint8 y = m_line; y < DISPLAY_Y; ++y)
    {
        for (quint8 x = 0; x < DISPLAY_X; ++x)
        {
            idx = getIndex( x,y );
            m_buf.setBit( idx, m_screen.testBit( idx ) );
        }
    }
    m_screen = m_buf;
}

void Display::moveRight()
{
    // Прокрутить изображение на экране на 4 пикселя вправо в режиме 128x64,
    // либо на 2 пикселя в режиме 64x32
    QBitArray m_buf = QBitArray( DISPLAY_X * DISPLAY_Y, false );
    quint16 idxOld;
    quint16 idxNew;
    quint8 shift = m_ExtendedMode ? 4 : 2;

    for (quint8 x = shift; x < DISPLAY_X; ++x)
    {
        for (quint8 y = 0; y < DISPLAY_Y; ++y)
        {
            idxOld = getIndex( x - shift, y );
            idxNew = getIndex( x , y );
            m_buf.setBit( idxNew, m_screen.testBit( idxOld ) );
        }
    }
    m_screen = m_buf;
}

void Display::moveLeft()
{
    // Прокрутить изображение на экране на 4 пикселя влево в режиме 128x64,
    // либо на 2 пикселя в режиме 64x32
    QBitArray m_buf = QBitArray( DISPLAY_X * DISPLAY_Y, false );
    quint16 idxOld;
    quint16 idxNew;
    quint8 shift = m_ExtendedMode ? 4 : 2;

    for (quint8 x = shift; x < DISPLAY_X - shift; ++x)
    {
        for (quint8 y = 0; y < DISPLAY_Y; ++y)
        {
            idxOld = getIndex( x, y );
            idxNew = getIndex( x - shift , y );
            m_buf.setBit( idxNew, m_screen.testBit( idxOld ) );
        }
    }
    m_screen = m_buf;

}

quint16 Display::getIndex(quint8 x, quint8 y)
{
    quint16 val = x + ( y*DISPLAY_X );
    return ( val > MAX_DISPLAY_SIZE )
            ? MAX_DISPLAY_SIZE
            : val;
}

bool Display::drawPixel(quint8 x, quint8 y, bool pixel)
{
    bool existPixel = false;
    quint16 idx = getIndex ( x, y ) ;

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
