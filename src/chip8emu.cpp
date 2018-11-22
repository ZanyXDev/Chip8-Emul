#include "chip8emu.h"

Chip8Emu::Chip8Emu(QObject *parent)
    : QObject(parent)
{
    initDevice();

    connect(&m_timer,&QTimer::timeout,this,&Chip8Emu::execute);
}

void Chip8Emu::loadData2Memory(QByteArray &data)
{
    initDevice();
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

    m_timer.start();
}

void Chip8Emu::stopEmulation()
{

}

void Chip8Emu::executeNextOpcode()
{
    QString asmTextString;

    unsigned short opCode  = ( (m_memory.at(PC) & 0x00FF) << 8 ) | ( m_memory.at(PC+1) & 0x00FF) ;
    unsigned short HI  = (opCode & 0xF000) >> 12;
    unsigned short LO  = (opCode & 0x000F);
    unsigned short X   = (opCode & 0x0F00) >> 8;
    unsigned short Y   = (opCode & 0x00F0) >> 4;
    unsigned short KK  = (opCode & 0x00FF);
    unsigned short NNN = (opCode & 0x0FFF);

    if (PC > m_memory.size() - 2)
    {
        emit finishExecute();
        return;
    }

    if ( !waitKeyPressed )
    {
        switch  ( HI ) {
        case 0x0:
            if ( KK == 0xE0 ){ // * 00E0 CLS   Очистить экран
                asmTextString.append(QString("CLS \t ; Clear screen"));
            }

            if ( KK == 0xEE ){ // * 00EE RET   Возвратиться из подпрограммы
                asmTextString.append(QString("RET \t ; Return sub-routine"));
            }

            //-------------------- Инструкции Super CHIP ----------------------------------------------
            if ( X == 0xC) { // 00Cn SCD n Прокрутить изображение на экране на n строк вниз
                asmTextString.append(QString("SCD 0x%1 \t ; Scroll down %1 lines").arg(LO,0,16));
            }

            if ( X == 0xF ) {
                switch ( Y ){
                case 0xB: // SCR Прокрутить изображение на экране на 4 пикселя вправо в режиме 128x64, либо на 2 пикселя в режиме 64x32
                    asmTextString.append(QString("SCR ; Scroll right on 4 (or 2 ) pixels"));
                    break;
                case 0xC: // SCL Прокрутить изображение на экране на 4 пикселя влево в режиме 128x64, либо на 2 пикселя в режиме 64x32
                    asmTextString.append(QString("SCL ; Scroll left on 4 (or 2 ) pixels"));
                    break;
                case 0xD: // EXIT Завершить программу
                    asmTextString.append(QString("EXIT ; Shutdown programm"));
                    break;
                case 0xE:  // LOW Выключить расширенный режим экрана. Переход на разрешение 64x32
                    asmTextString.append(QString("LOW ; Extend mode OFF"));
                    break;
                case 0xF:  // HIGH Включить расширенный режим экрана. Переход на разрешение 128x64
                    asmTextString.append(QString("HIGH ; Extend mode ON"));
                    break;
                default:
                    break;
                }
            }
            // 0nnn SYS nnn Перейти на машинный код RCA 1802 по адресу nnn. Эта инструкция была только
            // в самой первой реализации CHIP-8. В более поздних реализациях и эмуляторах не используется.
            break;
        case 0x1: // 1nnn JP nnn Перейти по адресу nnn
            asmTextString.append(QString("JP 0x%1 \t ; Jump to 0x%1 address").arg( NNN,0,16 ));
            break;
        case 0x2: // CALL nnn Вызов подпрограммы по адресу nnn
            asmTextString.append(QString("CALL 0x%1 \t ; Call sub-routine from 0x%1 address").arg( NNN,0,16 ));
            break;
        case 0x3: // 3xkk SE Vx, kk Пропустить следующую инструкцию, если регистр Vx = kk
            asmTextString.append(QString("SE V%1, 0x%2 \t ; Skip next instruction if V%1 = 0x%2").arg( X, 0, 16 ).arg( KK,0,16 ));
            break;
        case 0x4: // 4xkk SNE Vx, kk Пропустить следующую инструкцию, если регистр Vx != kk
            asmTextString.append(QString("SNE V%1, 0x%2 \t ; Skip next instruction if V%1 != 0x%2").arg( X, 0, 16 ).arg( KK, 0, 16 ));
            break;
        case 0x5: // 5xy0 SE Vx, Vy Пропустить следующую инструкцию, если Vx = Vy
            if ( LO == 0x0){
                asmTextString.append(QString("SE V%1, V%2 \t ; Skip next instruction if V%1 = V%2").arg( X,0,16 ).arg( Y,0,16 ));
            }else {
                asmTextString.append( QString("opCode 0x%1 is invalid").arg( opCode, 0, 16 ));
            }
            break;
        case 0x6: // 6xkk LD Vx, kk Загрузить в регистр Vx число kk, т.е. Vx = kk
            asmTextString.append(QString("LD V%1, 0x%2 \t ; Load 0x%2 to register V%1 ").arg( X, 0, 16 ).arg( KK, 0, 16 ));
            setRegister( X, KK );
            break;
        case 0x7: // 7xkk ADD Vx, kk Установить Vx = Vx + kk
            asmTextString.append(QString("ADD V%1, 0x%2 \t ; V%1 = V%1 + 0x%2").arg( X,0,16 ).arg( KK, 0, 16 ));
            break;
        case 0x8:
            switch ( LO ){
            case 0x0: // 8xy0 LD Vx, Vy Установить Vx = Vy
                asmTextString.append(QString("LD V%1, V%2 \t ; Set register V%1 = V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0x1: // 8xy1 OR Vx, Vy Выполнить операцию дизъюнкция (логическое “ИЛИ”) над значениями регистров Vx и Vy,
                // результат сохранить в Vx. Т.е. Vx = Vx | Vy
                asmTextString.append(QString("OR V%1, V%2 \t ; Set register V%1 = V%1 | V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0x2: // 8xy2 AND Vx, Vy Выполнить операцию конъюнкция (логическое “И”) над значениями регистров Vx и Vy,
                // результат сохранить в Vx. Т.е. Vx = Vx & Vy
                asmTextString.append(QString("OR V%1, V%2 \t ; Set register V%1 = V%1 & V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0x3: // 8xy3 XOR Vx, Vy Выполнить операцию “исключающее ИЛИ” над значениями регистров Vx и Vy,
                // результат сохранить в Vx. Т.е. Vx = Vx ^ Vy
                asmTextString.append(QString("XOR V%1, V%2 \t ; Set register V%1 = V%1 ^ V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0x4: // 8xy4 ADD Vx, Vy Значения Vx и Vy суммируются. Если результат больше, чем 8 бит (т.е.> 255)
                // VF устанавливается в 1, иначе 0. Только младшие 8 бит результата сохраняются в Vx. Т.е. Vx = Vx + Vy
                asmTextString.append(QString("ADD V%1, V%2 \t ; Set register V%1 = V%1 + V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0x5: // 8xy5 SUB Vx, Vy Если Vx >= Vy, то VF устанавливается в 1, иначе 0. Затем Vy вычитается из Vx,
                // а результат сохраняется в Vx. Т.е. Vx = Vx - Vy
                asmTextString.append(QString("SUB V%1, V%2 \t ; Set register V%1 = V%1 - V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0x6: // 8xy6 SHR Vx {, Vy} Операция сдвига вправо на 1 бит. Сдвигается регистр Vx. Т.е. Vx = Vx >> 1.
                // До операции сдвига выполняется следующее: если младший бит (самый правый) регистра Vx равен 1, то VF = 1, иначе VF = 0
                asmTextString.append(QString("SHR V%1 {, V%2} \t ; Set register V%1 = V%1 >> 1 ").arg(X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0x7: // 8xy7 SUBN Vx, Vy Если Vy >= Vx, то VF устанавливается в 1, иначе 0. Тогда Vx вычитается из Vy,
                // и результат сохраняется в Vx. Т.е. Vx = Vy - Vx
                asmTextString.append(QString("SUBN V%1, V%2 \t ; IF V%2 > V%1 { SET VF = 1} ELSE { SET VF = 0, set register V%1 = V%2 - V%1 } ").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            case 0xE: // 8xyE SHL Vx {, Vy} Операция сдвига влево на 1 бит. Сдвигается регистр Vx. Т.е. Vx = Vx << 1.
                // До операции сдвига выполняется следующее: если младший бит (самый правый) регистра Vx равен 1, то VF = 1, иначе VF = 0
                asmTextString.append(QString("SHL V%1 {, V%2} \t ; Set register V%1 = V%1 << 1 ").arg( X,0,16 ).arg( Y, 0, 16 ));
                break;
            default:
                break;
            }
            break;
        case 0x9: // 9xy0 SNE Vx, Vy Пропустить следующую инструкцию, если Vx != Vy
            if ( LO == 0x0) {
                asmTextString.append(QString("SNE V%1, V%2 \t ; Skip next instruction if V%1 != V%2").arg( X,0,16 ).arg( Y, 0, 16));
            }else {
                asmTextString.append( QString("opCode 0x%1 is invalid").arg( opCode, 0, 16 ));
            }
            break;
        case 0xA: // Annn LD I, nnn Значение регистра I устанавливается в nnn
            asmTextString.append(QString("LD I, 0x%1 \t ; set register I to 0x%1").arg( NNN,0,16 ));
            break;
        case 0xB:// Bnnn JP V0, nnn Перейти по адресу nnn + значение в регистре V0.
            asmTextString.append(QString("JP V0, 0x%1 \t ; Jump to address V0 + %1").arg( NNN,0,16 ));
            break;
        case 0xC: //Cxkk RND Vx, kk Устанавливается Vx = (случайное число от 0 до 255) & kk
            asmTextString.append(QString("RND V%1, 0x%2 \t ; Set register V%1 = random {0,255} & 0x%2 ").arg( X,0,16 ).arg( KK,0,16 ));
            break;
        case 0xD: /** Dxyn DRW Vx, Vy, n Нарисовать на экране спрайт. Эта инструкция считывает n байт по адресу
                    * содержащемуся в регистре I и рисует их на экране в виде спрайта c координатой Vx, Vy.
                    * Спрайты рисуются на экран по методу операции XOR, то есть если в том месте где мы рисуем спрайт
                    * уже есть нарисованные пиксели - они стираются, если их нет - рисуются. Если хоть один пиксель был стерт,
                    * то VF устанавливается в 1, иначе в 0.
                    **/
#ifdef DEBUG
            drawSprite(5,5,9);
#endif
            asmTextString.append(QString("DRW V%1, 0x%2, 0x%3 \t ; Draw sprite (0x%3 bytes) in the pos( 0x%1, 0x%2 ) ").arg( X,0,16 ).arg( Y,0,16 ).arg( LO,0,16 ));
            break;
        case 0xE:
            if ( KK == 0x9E){ // Ex9E SKP Vx Пропустить следующую команду если клавиша, номер которой хранится в регистре Vx, нажата
                asmTextString.append(QString("SKP V%1 \t ; Skip next instruction if key number (save register V%1) pressed ").arg( X,0,16 ) );
            }
            if ( KK == 0xA1){ // ExA1 SKNP Vx Пропустить следующую команду если клавиша, номер которой хранится в регистре Vx, не нажата
                asmTextString.append(QString("SKNP V%1 \t ; Skip next instruction if key number (save register V%1) not pressed ").arg( X,0,16 ) );
            }
            break;
        case 0xF:
            switch ( KK ) {
            case 0x07:// Fx07 LD Vx, DT Скопировать значение таймера задержки в регистр Vx
                asmTextString.append(QString("LD V%1, DT \t ; Set register V%1 = DATA_TIMER ").arg( X,0,16 ) );
                break;
            case 0x0A:
                /**
                 * Fx0A LD Vx, K Ждать нажатия любой клавиши.
                 * Как только клавиша будет нажата записать ее номер в регистр Vx и перейти к выполнению следующей инструкции.
                 */
                asmTextString.append(QString("LD V%1, K \t ; Wait any key pressed, after SET register V%1 = KEY_NUMBER ").arg( X,0,16 ) );
                currentRegister = X;
                waitKeyPressed = true;
                break;
            case 0x15: // Fx15 LD DT, Vx Установить значение таймера задержки равным значению регистра Vx
                asmTextString.append(QString("LD DT, V%1 \t ; Set register DATA_TIMER = V%1 ").arg( X,0,16 ) );
                break;
            case 0x18:// Fx18 LD ST, Vx Установить значение звукового таймера равным значению регистра Vx
                asmTextString.append(QString("LD ST, V%1 \t ; Set register SOUND_TIMER = V%1 ").arg( X,0,16 ) );
                break;
            case 0x1E: // Fx1E ADD I, Vx Сложить значения регистров I и Vx, результат сохранить в I. Т.е. I = I + Vx
                asmTextString.append(QString("ADD I, V%1 \t ; Set register I = I + V%1 ").arg( X,0,16 ) );
                break;
            case 0x29: // Fx29 LD F, Vx Используется для вывода на экран символов встроенного шрифта размером 4x5 пикселей.
                // Команда загружает в регистр I адрес спрайта, значение которого находится в Vx.
                // Например, нам надо вывести на экран цифру 5. Для этого загружаем в Vx число 5.
                // Потом команда LD F, Vx загрузит адрес спрайта, содержащего цифру 5, в регистр I
                asmTextString.append(QString("LD F, V%1 \t ; Show sprite font ").arg( X,0,16 ) );
                break;

            case 0x30: // Fx30 LD HF, Vx Работает подобно команде Fx29, только загружает спрайты размером 8x10 пикселей
                asmTextString.append(QString("LD HF, V%1 \t ; Show sprite font 8x10 pixel ").arg( X,0,16 ) );
                break;
            case 0x33: // Fx33 LD B, Vx Сохранить значение регистра Vx в двоично-десятичном (BCD) представлении по адресам I, I+1 и I+2
                asmTextString.append(QString("LD B, V%1 \t ; Save register V%1 in memory {binary-decimal presentation},  address register I, I+1, I+2 ").arg( X,0,16 ) );
                break;
            case 0x55: // Fx55 LD [I], Vx Сохранить значения регистров от V0 до Vx в памяти, начиная с адреса находящегося в I
                asmTextString.append(QString("LD [I], V%1 \t ; Save registers {V0, V%1} in memory, start address = register I ").arg( X,0,16 ) );
                break;
            case 0x65: // Fx65 LD Vx, [I] Загрузить значения регистров от V0 до Vx из памяти, начиная с адреса находящегося в I
                asmTextString.append(QString("LD V%1, [I] \t ; Load registers {V0, V%1} from memory, start address = register I ").arg( X,0,16 ) );
                break;
            case 0x75: //Fx75 LD R, Vx Сохранить регистры V0 - Vx в пользовательских флагах [RPL](http://en.wikipedia.org/wiki/RPL_(programming_language))
                asmTextString.append(QString("LD R, V%1 \t ; Save registers {V0, V%1} in the users flag  [RPL](http://en.wikipedia.org/wiki/RPL_(programming_language)").arg( X,0,16 ) );
                break;
            case 0x85: //Fx85 LD Vx, R Загрузить регистры V0 - Vx из пользовательских флагов RPL
                asmTextString.append(QString("LD V%1, R \t ; Load registers {V0, V%1} from the users flag [RPL](http://en.wikipedia.org/wiki/RPL_(programming_language)").arg( X, 0,16) );
                break;
            default:
                break;
            }
            break;
        default:
            asmTextString.append(QString("\t ;Don't decode ERROR"));
            break;
        }

        emit showDecodeOpCode ( QString("0x%1:\t%2\t%3").arg(PC,0,16).arg(opCode,0,16).arg(asmTextString) );

        PC+=2;
    }

    return;
}

void Chip8Emu::decreaseTimers()
{
    if (delay_timer > 0)
    {
        --delay_timer;
    }
}



void Chip8Emu::setRegister(unsigned short m_reg, unsigned short m_value)
{
    if (m_reg < 16)
    {
        m_regs[m_reg] = m_value ;
    }

}

unsigned short Chip8Emu::getRegister(unsigned short m_reg)
{
    unsigned short value = 0;
    if (m_reg < 16)
    {
        value = m_regs.at(m_reg) ;
    }
    return value;
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
    m_regs.fill(0x0,16);
    m_screen.fill(false, DISPLAY_X * DISPLAY_Y);
    m_keys.fill(false, KEY_PAD);   // All keys unPressed
    m_ExtendedMode = false;
    m_ElapsedTime = 0;
    waitKeyPressed = false;
    currentRegister = 0;
}



void Chip8Emu::changeKeyState(int key, bool state)
{
     m_keys.setBit(key, state);

     if (waitKeyPressed)
     {
         setRegister(currentRegister, key);
         waitKeyPressed = false;
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


















