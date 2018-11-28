#ifndef CHIP8EMU_H
#define CHIP8EMU_H

#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QBitArray>
#include <QVector>
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
    void stepEmulation();
    void stopEmulation();   
    void changeKeyState(quint8 key, bool state);

private slots:
    void execute();
private:
    void initDevice();
    void executeNextOpcode();
    void decreaseTimers();

    void setRegister(quint8 m_reg, quint8 m_value);
    quint16 getRegister(quint8 m_reg);

    void setRegI( quint16 m_value );
    quint16 getRegI();

    /**
     * @brief drawSprite
     * @param vx
     * @param vy
     * @param n
     * @return
     * @note реализует поведение опкода DXYN.
     * Она рисует в массив m_screen спрайт находящийся по смещению I.
     * Координаты спрайта берутся из регистров VX и VY.
     * Один байт по смещению I представляет одну строку спрайта для CHIP-8, и полстроки для SCHIP (при N=0).
     * Каждый бит в этом байте представляет отдельный пиксель.
     * Количество строк N извлекается из опкода. Если N=0, то в спрайте 16 строк.
     * Также не стоит забывать о том, что если мы рисуем пиксель “поверх” уже существующего
     * пикселя, то эта точка экрана очистится, а регистр VF установится в 1. То есть рисуем методом XOR.
     */
    void drawSprite(quint8 vx, quint8 vy, quint8 n);
    void moveRight();
    void moveLeft();
    void moveDown( quint8 m_line );

    quint8 getRealKey (quint8 m_emu_key);

    /**
     * @brief getSumCF sum x and y with carry flag
     * @param x
     * @param y
     * @return quint8 sum
     */
    quint8 getSumCF( quint8 x, quint8 y ) ;
    /**
     * @brief Save register Vx in memory {binary-decimal presentation},  address register I, I+1, I+2
     * @param m_reg_val Value from register Vx
     * @note
     */
    void saveBCDRegToI(quint8 m_reg_val);

    /**
     * @brief saveRegToMemory
     * @param m_reg_val
     * @note opCode Fx55 LD [I], Vx Save registers {V0, V%1} in memory, start address = register I
     */
    void saveRegToMemory (quint8 m_reg_val);

    QTimer m_timer;
    QByteArray m_memory;    // 4k ram memory
    QByteArray m_regs;      // 16 registers 8bit size;
    QByteArray m_smallFont; // size 16x5 small font
    QByteArray m_bigFont;   // size 16x10 big font

    QVector<quint16> m_stack;     // deep 16 levels;
    QBitArray m_screen;
    QBitArray m_keys;

    quint16 PC;             // mem offset counter
    quint16 regI;           // 16bit address register I

    quint8 delay_timer;     // 8bit delay timer;
    quint8 sound_timer;     // 8bit sound timer;

    quint16 opcode_count;
    quint32 cycles_per_second;
    quint32 m_ElapsedTime;

    bool m_ExtendedMode;    // Chip8 (false) or SuperChip (true) mode


};

#endif // CHIP8EMU_H
