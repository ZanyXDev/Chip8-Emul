#include "keyboard.h"

Keyboard::Keyboard(QObject *parent) : QObject(parent)
{

}

bool Keyboard::isPressed(quint8 key)
{
    return m_keys.contains( key );
}

bool Keyboard::isNotPressed(quint8 key)
{
    return !m_keys.contains( key );
}

quint8 Keyboard::anyKeyPressed()
{
    return ( m_keys.isEmpty() )
            ? KEY_PAD
            : m_keys.toList().first();
}

void Keyboard::changeKeyState(quint8 key, bool state)
{
    QMutexLocker locker(&mutex);
    if (state)
    {
        m_keys.insert( key );
    }
    else
    {
        m_keys.remove( key );
    }

}
