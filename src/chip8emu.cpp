#include "chip8emu.h"

Chip8Emu::Chip8Emu(QObject *parent)
    : QObject(parent)
    , PC(START_ADDR)
{
    PC = START_ADDR;
    regI = START_ADDR;
    m_memory.fill(0x0,RAM_SIZE);   // clear 4k ram memory
    m_stopped = false;
    opcode_count = 0 ;

    delay_timer = 0;               // clear delay timer;
    sound_timer = 0;               // clear sound timer;
    m_screen = QBitArray(DISPLAY_X * DISPLAY_Y,false);  // clear screen map

}

void Chip8Emu::loadData2Memory(QByteArray &data)
{
    qDebug() << "load: " << data.size() << " bytes";
    if ( !data.isEmpty() &&
         ( data.size() <= RAM_SIZE - START_ADDR)  ) {
        m_memory.insert(START_ADDR,data);

        emit ReadyToWork(true);
        m_stopped = false;
    }
}

void Chip8Emu::startEmulation()
{

    QElapsedTimer et;
    et.start();

    opcode_count = 0;
    int cycles_per_second;

    while (!m_stopped) {
        QCoreApplication::processEvents ( QEventLoop::AllEvents );

        if (m_mode == Chip8Emu::SuperMode) {
            cycles_per_second = 10; // execute 600 opcodes per second
        } else {
            cycles_per_second = 30; // 1800 opcodes per second
        }
        if (opcode_count < cycles_per_second) {
            executeNextOpcode();
            opcode_count++;
        }

        //decrease timers every 1/60sec and redraw screen
        if (et.hasExpired(1000/60)) {
            decreaseTimers();
            et.restart();
            emit updateScreen(m_screen);
            opcode_count = 0;
        }
        //if (emu->stop == true) closeRom();
    }

}

void Chip8Emu::stopEmulation()
{
    m_stopped = true;
}

void Chip8Emu::executeNextOpcode()
{
    //debugShowTime();
    drawSprite(5,5,9);
}

void Chip8Emu::decreaseTimers()
{
    if (delay_timer > 0) {
        --delay_timer;
    }
}

void Chip8Emu::debugShowTime()
{
    // int idx = DISPLAY_X * DISPLAY_Y;
    // for (int i=0; i< 24; ++i){
    //        m_screen.toggleBit(QRandomGenerator::global()->bounded( idx ));
    //  }

}

/**
 * @brief drawSprite
 * @param idx
 * @param sprite
 * @return
 * @note реализует поведение опкода DXYN. Она рисует в массив m_screen
 * спрайт находящийся по смещению I. Координаты спрайта берутся из регистров VX и VY.
 * Один байт по смещению I представляет одну строку спрайта для CHIP-8,
 * и полстроки для SCHIP (при N=0). Каждый бит в этом байте представляет отдельный пиксель.
 * Количество строк N извлекается из опкода. Если N=0, то в спрайте 16 строк.
 * Также не стоит забывать о том, что если мы рисуем пиксель “поверх” уже существующего
 * пикселя, то эта точка экрана очистится, а регистр VF установится в 1.
 * То есть рисуем методом XOR.
 */
bool Chip8Emu::drawSprite(int vx,int vy, int n)
{
    bool vf_flag = false;
    bool showPixel;
    unsigned short maxLine;
    unsigned short drw;
    unsigned short idx ;
#ifdef DEBUG
/**
  * Test sprite 8x9 pixel
  * DB $.111.... 0x70
  * DB $1...1... 0x88
  * DB $....1... 0x8
  * DB $....1... 0x8
  * DB $...1.... 0x10
  * DB $..1..... 0x20
  * DB $.1...... 0x40
  * DB $1....... 0x80
  * DB $11111... 0xf8
*/
    m_memory[regI] = 0x70;
    m_memory[regI+1] = 0x88;
    m_memory[regI+2] = 0x8;
    m_memory[regI+3] = 0x8;
    m_memory[regI+4] = 0x10;
    m_memory[regI+5] = 0x20;
    m_memory[regI+6] = 0x40;
    m_memory[regI+7] = 0x80;
    m_memory[regI+8] = 0xf8;
#endif

    if ( 0 == n){ // check how many rows draw
        maxLine = 16;
    }else {
        maxLine = n;
    }

    for (int row = 0; row < maxLine; ++row){
        drw = m_memory.at(regI+row);
        for (int col = 0; col < 8; ++col){
            showPixel = drw  & (1 << (7 - col));
            idx = (vx + col) + ((vy + row) * DISPLAY_X);

            if ( m_screen.testBit( idx ) && !showPixel){
                vf_flag = true;
            }

            m_screen.setBit( idx, showPixel);
        }
    }

    return vf_flag;
}
