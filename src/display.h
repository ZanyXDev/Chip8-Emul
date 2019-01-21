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
    void moveDown(quint8 m_line);
    void moveRight();
    void moveLeft();

    bool drawPixel( quint16 idx, bool pixel);
    bool getPixel( quint16 idx );
    void setHiResMode( bool mode);
    bool getHiResMode();
signals:

public slots:

private:
    QBitArray m_screen;
    bool m_ExtendedMode;    // Chip8 (false) or SuperChip (true) mode
};

#endif // DISPLAY_H
