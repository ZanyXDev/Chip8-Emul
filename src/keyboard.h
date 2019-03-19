#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QSet>
#include <QMutex>
#include <QMutexLocker>

#include "mydefs.h"

class Keyboard : public QObject
{
    Q_OBJECT
public:
    explicit Keyboard(QObject *parent = nullptr);
    bool isPressed( quint8 key );
    bool isNotPressed( quint8 key );
    quint8 anyKeyPressed();
signals:

public slots:
    void changeKeyState(quint8 key, bool state);

private:
    QSet<quint8> m_keys;
    QMutex mutex;
};

#endif // KEYBOARD_H
