#ifndef DISPLAY_H
#define DISPLAY_H
#include <QBitArray>

#include "mydefs.h"

class Display : public QObject
{
    Q_OBJECT
public:
    explicit Display(QObject *parent = nullptr);
    void clear();
signals:

public slots:
private:
    QBitArray m_screen;
};

#endif // DISPLAY_H
