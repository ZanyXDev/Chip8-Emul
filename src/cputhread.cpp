#include "cputhread.h"

CPUThread::CPUThread(QObject *parent) : QObject(parent)
{
    PC = START_ADDR;
    regI = START_ADDR;
    m_memory.fill(0x0,RAM_SIZE);   // clear 4k ram memory
    opCodeCount = 0 ;
    m_WorkMode = false;

    delay_timer = 0;               // clear delay timer;
    sound_timer = 0;               // clear sound timer;
    m_screen = QBitArray(DISPLAY_X * DISPLAY_Y,false);  // clear screen map
}

bool CPUThread::running() const
{
    return m_running;
}

bool CPUThread::getWorkMode() const
{
    return m_WorkMode;
}

QBitArray CPUThread::getScreen() const
{
    return m_screen;
}

QByteArray CPUThread::getMemory() const
{
    return m_memory;
}


// Самый важный метод, в котором будет выполняться "полезная" работа объекта
void CPUThread::run()
{
    QElapsedTimer et;
    et.start();
    opCodeCount = 0;
    int cycles_per_second;
    // Переменная m_running отвечает за работу объекта в потоке.
    // При значении false работа завершается
    while (m_running){

        if ( m_WorkMode ) {
            cycles_per_second = 10; // execute 600 opcodes per second SuperCHIP
        } else {
            cycles_per_second = 30; // 1800 opcodes per second
        }
        if (opCodeCount < cycles_per_second) {
            executeNextOpcode();
            opCodeCount++;
        }
        //decrease timers every 1/60sec and redraw screen
        if (et.hasExpired(1000/60)) {
            decreaseTimers();
            et.restart();
            emit screenChanged( m_screen );
            opCodeCount = 0;
        }
    }
    emit finished();
}

void CPUThread::setRunning(bool running)
{
    if (m_running == running){
        return;
    }
    m_running = running;
    emit runningChanged(running);
}

void CPUThread::setMemory(QByteArray rom)
{
    qDebug() << "load: " << rom.size() << " bytes";
    if ( !rom.isEmpty() &&
         ( rom.size() <= RAM_SIZE - START_ADDR)  ) {
        m_memory.insert(START_ADDR,rom);
    }
}

void CPUThread::setWorkMode(bool mode)
{
    m_WorkMode = mode;
}

void CPUThread::executeNextOpcode()
{
    //debugShowTime();
    drawSprite(5,5,9);
}

void CPUThread::decreaseTimers()
{
    if (delay_timer > 0) {
        --delay_timer;
    }
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
bool CPUThread::drawSprite(int vx,int vy, int n)
{
    bool vf_flag = false;
    bool showPixel;
    unsigned short maxLine;
    unsigned short drw;
    unsigned short idx ;

#ifdef DEBUG
    m_memory[regI]   = 0x70;
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
