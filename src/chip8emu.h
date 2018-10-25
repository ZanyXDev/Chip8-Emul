#ifndef CHIP8EMU_H
#define CHIP8EMU_H

#include <QObject>

class Chip8Emu : public QObject
{
    Q_OBJECT
public:
    explicit Chip8Emu(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CHIP8EMU_H