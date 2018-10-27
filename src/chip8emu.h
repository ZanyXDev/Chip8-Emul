#ifndef CHIP8EMU_H
#define CHIP8EMU_H

#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QBitArray>
#include <QRandomGenerator>

#define RAM_SIZE 4096
#define START_ADDR 0x200
#define DISPLAY_X 64
#define DISPLAY_Y 32

#define DEBUG 1
class Chip8Emu : public QObject
{
    Q_OBJECT
public:
    enum WorkModeFlag {
        Emulation = 0x1,
        Debuging  = 0x2,
        SuperMode = 0x5,
        SuperModeDebug =  Debuging | SuperMode,
        UnDefined = 0xf
    };
    Q_DECLARE_FLAGS(WorkMode, WorkModeFlag)

    explicit Chip8Emu(QObject *parent = nullptr);

signals:
    void ReadyToWork( bool flag);
    void updateScreen( QBitArray screen );

public slots:
    void loadData2Memory(QByteArray &data);
    void startEmulation();
    void stopEmulation();
private:

    unsigned short PC;   // mem offset counter
    unsigned short regI; // address register I
    QByteArray m_memory; // 4k ram memory
    QBitArray m_screen;

    int opcode_count;
    bool m_stopped;
    WorkModeFlag m_mode;
    short delay_timer;         // delay timer;
    short sound_timer;         // sound timer;

    bool drawSprite(int vx,int vy, int n);
    void executeNextOpcode();
    void decreaseTimers();
    void debugShowTime();
};

#endif // CHIP8EMU_H
