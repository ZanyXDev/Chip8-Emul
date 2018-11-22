#include "chip8emu.h"

Chip8Emu::Chip8Emu(QObject *parent)
    : QObject(parent)
{
    initDevice();

    connect(&m_timer,&QTimer::timeout,this,&Chip8Emu::execute);
}

void Chip8Emu::loadData2Memory(QByteArray &data)
{
    qDebug() << "load: " << data.size() << " bytes";
    if ( !data.isEmpty() &&
         ( data.size() <= RAM_SIZE - START_ADDR)  )
    {
        m_memory.insert(START_ADDR,data);
        emit ReadyToWork(true);
    }
}

void Chip8Emu::startEmulation()
{    
    initDevice();
    m_timer.start();
}

void Chip8Emu::stopEmulation()
{

}

void Chip8Emu::executeNextOpcode()
{
    if ( !waitKeyPressed )
    {
        PC++;
    }else
    {
        drawSprite(5,5,9);
    }
}

void Chip8Emu::decreaseTimers()
{
    if (delay_timer > 0)
    {
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

    if ( 0 == n)
    { // check how many rows draw
        maxLine = 16;
    }else
    {
        maxLine = n;
    }

    for (int row = 0; row < maxLine; ++row)
    {
        drw = m_memory.at(regI+row);
        for (int col = 0; col < 8; ++col)
        {
            showPixel = drw  & (1 << (7 - col));
            idx = (vx + col) + ((vy + row) * DISPLAY_X);

            if ( m_screen.testBit( idx ) && !showPixel)
            {
                vf_flag = true;
            }

            m_screen.setBit( idx, showPixel);
        }
    }

    return vf_flag;
}

void Chip8Emu::initDevice()
{
    PC = START_ADDR;               // set mem offset counter
    regI = START_ADDR;
    delay_timer = 0;               // clear delay timer;
    sound_timer = 0;               // clear sound timer;
    opcode_count = 0 ;
    m_memory.fill(0x0,RAM_SIZE);   // clear 4k ram memory
    m_screen.fill(false, DISPLAY_X * DISPLAY_Y);
    m_ExtendedMode = false;
    m_ElapsedTime = 0;
    waitKeyPressed = true;
}

/**
 * 76543210     76543210     76543210     76543210     76543210     76543210
..1.....0x20 .111....0x70 .111....0x70 ...1....0x20 11111...0xf8 ........
.11.....0x60 1...1...0x88 1...1...0x88 ..11....0x60 1.......0x80 .111....0x70
1.1.....0xa0 ....1...0x8  ....1...0x8  ..11....0x60 1.......0x80 .111....0x70
..1.....0x20 ....1...0x8  ....1...0x8  .1.1....0x50 1.......0x80 ........
..1.....0x20 ...1....0x10 ..11....0x30 .1.1....0x50 .111....0x70 ........
..1.....0x20 ..1.....0x20 ....1...0x8  1..1....0x88 ....1...0x8  ........
..1.....0x20 .1......0x40 ....1...0x8  11111...0xf8 ....1...0x8  .111....0x70
..1.....0x20 1.......0x80 1...1...0x88 ...1....0x10 1...1...0x88 .111....0x70
11111...0xf8 11111...0xf8 .111....0x70 ..111...0x70 .111....0x70 ........

.111....0x70 11111...0xf8 .111....0x70 .111....0x70 .111....0x70 ........
1...1...0x88 1...1...0x88 1...1...0x88 1...1...0x88 1...1...0x88 ........
1.......0x80 ....1...0x8  1...1...0x88 1...1...0x88 1...1...0x88 ........
1.......0x80 ...1....0x10 1...1...0x88 1...1...0x88 1..11...0x98 ........
1111....0xf8 ..1.....0x20 .111....0x70 .111....0x70 1.1.1...0xa8 ........
1...1...0x88 .1......0x40 1...1...0x88 ....1...0x8  11..1...0xc8 ........
1...1...0x88 1.......0x80 1...1...0x88 ....1...0x8  1...1...0x88 ........
1...1...0x88 1.......0x80 1...1...0x88 1...1...0x88 1...1...0x88 ..111...0x70
.111....0x70 1.......0x80 .111....0x70 .111....0x70 .111....0x70 ..111...0x70

*/

void Chip8Emu::pressedKey(int key)
{
    /**
        * Key mappings
        * The original CHIP-8 keypad | This is emulated as follows
        * +-+-+-+-+                  | +-+-+-+-+
        * |1|2|3|C|                  | |1|2|3|4|
        * +-+-+-+-+                  | +-+-+-+-+
        * |4|5|6|D|                  | |Q|W|E|R|
        * +-+-+-+-+                  | +-+-+-+-+
        * |7|8|9|E|                  | |A|S|D|F|
        * +-+-+-+-+                  | +-+-+-+-+
        * |A|0|B|F|                  | |Z|X|C|V|
        * +-+-+-+-+                  | +-+-+-+-+
    */
    switch (key) {
    case Qt::Key_1:
        waitKeyPressed = false;
        break;
    case Qt::Key_2:
        waitKeyPressed = false;
        break;
    case Qt::Key_3:
        waitKeyPressed = false;
        break;
    case Qt::Key_4:
        waitKeyPressed = false;
        break;
    case Qt::Key_Q:
        waitKeyPressed = false;
        break;
    case Qt::Key_W:
        waitKeyPressed = false;
        break;
    case Qt::Key_E:
        waitKeyPressed = false;
        break;
    case Qt::Key_R:
        waitKeyPressed = false;
        break;
    case Qt::Key_A:
        waitKeyPressed = false;
        break;
    case Qt::Key_S:
        waitKeyPressed = false;
        break;
    case Qt::Key_D:
        waitKeyPressed = false;
        break;
    case Qt::Key_F:
        waitKeyPressed = false;
        break;
    case Qt::Key_Z:
        waitKeyPressed = false;
        break;
    case Qt::Key_X:
        waitKeyPressed = true;
        break;
    case Qt::Key_C:
        waitKeyPressed = false;
        break;
    case Qt::Key_V:
        waitKeyPressed = false;
        break;
    default:
        break;
    }
}

void Chip8Emu::execute()
{
    m_ElapsedTime++;

    if (PC > m_memory.size() - 2)
    {
        emit finishExecute();
        return;
    }

    //логическое выражение ? выражение 1 : выражение 2

    if (opcode_count < ( m_ExtendedMode ? LAPS_TYPE_1 : LAPS_TYPE_2) )
    {
        executeNextOpcode();
        opcode_count++;
    }

    //decrease timers every 1/60sec and redraw screen
    if (m_ElapsedTime > REFRESH_TIME )
    {
        decreaseTimers();

#ifdef DEBUG
        QString str = QString("%1 PC:%2")
                .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                .arg(PC,0,10);

        emit  showTime(str);
#endif
        emit updateScreen(m_screen);
        opcode_count = 0;
        m_ElapsedTime = 0;
    }
}


















