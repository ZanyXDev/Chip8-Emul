#include "display.h"

Display::Display(QObject *parent) : QObject(parent)
{

}

void Display::clear()
{
     m_screen = QBitArray( DISPLAY_X * DISPLAY_Y, false );
}
