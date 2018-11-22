#ifndef CHIP8EMU_H
#define CHIP8EMU_H

#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QBitArray>
#include <QRandomGenerator>
#include <QTimer>

#include "mydefs.h"

#ifdef DEBUG
#include <QTime>
#endif

class Chip8Emu : public QObject
{
    Q_OBJECT
public:
    explicit Chip8Emu(QObject *parent = nullptr);

signals:
    void ReadyToWork( bool flag);
    void updateScreen( QBitArray screen );
    void finishExecute();
    void showDecodeOpCode( const QString &asm_txt );

#ifdef DEBUG
    void showTime(const QString &m_time);
#endif

public slots:
    void loadData2Memory(QByteArray &data);
    void startEmulation();
    void stopEmulation();   
    void changeKeyState(int key, bool state);

private slots:
    void execute();
private:
    bool drawSprite(int vx,int vy, int n);

    void initDevice();
    void executeNextOpcode();
    void decreaseTimers();

    void setRegister(unsigned short m_reg, unsigned short m_value);
    unsigned short getRegister(unsigned short m_reg);

    QTimer m_timer;
    QByteArray m_memory;    // 4k ram memory
    QByteArray m_regs;      // 16 registers;
    QBitArray m_screen;
    QBitArray m_keys;

    unsigned short PC;      // mem offset counter
    unsigned short regI;    // address register I
    short delay_timer;      // delay timer;
    short sound_timer;      // sound timer;
    int opcode_count;
    int cycles_per_second;
    int m_ElapsedTime;
    unsigned short currentRegister;

    bool m_ExtendedMode;    // Chip8 (false) or SuperChip (true) mode
    bool waitKeyPressed;

};

#endif // CHIP8EMU_H
