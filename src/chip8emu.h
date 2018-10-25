#ifndef CHIP8EMU_H
#define CHIP8EMU_H

#include <QByteArray>
#include <QObject>

#define RAM_SIZE 4096
#define START_ADDR 0x200


class Chip8Emu : public QObject
{
    Q_OBJECT
public:
    explicit Chip8Emu(QObject *parent = nullptr);

signals:
    void ReadyToWork(bool);

public slots:
    void loadData2Memory(QByteArray &data);
private:
    unsigned short PC;   // mem offset counter
    QByteArray m_memory; // 4k ram memory

};

#endif // CHIP8EMU_H
