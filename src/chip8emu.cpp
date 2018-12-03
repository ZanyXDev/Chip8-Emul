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

void Chip8Emu::stepEmulation()
{
    this->execute();
    emit updateScreen(m_screen);
}

void Chip8Emu::stopEmulation()
{
    m_timer.stop();
}

void Chip8Emu::executeNextOpcode()
{
    if (PC > m_memory.size() - 2)
    {
        emit finishExecute();
        return;
    }


    QString asmTextString;

    unsigned short opCode  = ( (m_memory.at(PC) & 0x00FF) << 8 ) | ( m_memory.at(PC+1) & 0x00FF) ;
    unsigned short HI  = (opCode & 0xF000) >> 12;
    unsigned short LO  = (opCode & 0x000F);
    unsigned short X   = (opCode & 0x0F00) >> 8;
    unsigned short Y   = (opCode & 0x00F0) >> 4;
    unsigned short KK  = (opCode & 0x00FF);
    unsigned short NNN = (opCode & 0x0FFF);

    switch  ( HI )
    {
    case 0x0:
        if ( KK == 0xE0 )
        {
            // * 00E0 CLS   Очистить экран
            asmTextString.append(QString("CLS \t ; Clear screen"));
            m_screen.fill(false, DISPLAY_X * DISPLAY_Y);
        }

        if ( KK == 0xEE )
        {
            // * 00EE RET   Возвратиться из подпрограммы
            asmTextString.append(QString("RET \t ; Return sub-routine"));
            PC = m_stack.takeFirst();
        }

        if ( X == 0xC)
        {
            // 00Cn SCD n Прокрутить изображение на экране на n строк вниз
            asmTextString.append(QString("SCD 0x%1 \t ; Scroll down %1 lines").arg(LO,0,16));
            moveDown( LO );
        }

        if ( X == 0xF )
        {
            switch ( Y )
            {
            case 0xB:
                // 00FB SCR Прокрутить изображение на экране на 4 пикселя вправо в режиме 128x64, либо на 2 пикселя в режиме 64x32
                asmTextString.append(QString("SCR ; Scroll right on 4 (or 2 ) pixels"));
                moveRight();
                break;
            case 0xC:
                // 00FC SCL Прокрутить изображение на экране на 4 пикселя влево в режиме 128x64, либо на 2 пикселя в режиме 64x32
                asmTextString.append(QString("SCL ; Scroll left on 4 (or 2 ) pixels"));
                moveLeft();
                break;
            case 0xD: // EXIT Завершить программу
                asmTextString.append(QString("EXIT ; Shutdown programm"));
                break;
            case 0xE:
                // 00FE LOW Выключить расширенный режим экрана. Переход на разрешение 64x32
                asmTextString.append(QString("LOW ; Extend mode OFF"));
                m_ExtendedMode = false;
                break;
            case 0xF:
                // 00FF HIGH Включить расширенный режим экрана. Переход на разрешение 128x64
                asmTextString.append(QString("HIGH ; Extend mode ON"));
                m_ExtendedMode = true;
                break;
            default:
                break;
            }
        }
        // 0nnn SYS nnn Перейти на машинный код RCA 1802 по адресу nnn. Эта инструкция была только
        // в самой первой реализации CHIP-8. В более поздних реализациях и эмуляторах не используется.
        break;
    case 0x1:
        // 1nnn JP nnn Перейти по адресу nnn
        asmTextString.append(QString("JP 0x%1 \t ; Jump to 0x%1 address").arg( NNN,0,16 ));
        PC = NNN;
        PC-=2; // correct call and jump pointer
        break;
    case 0x2:
        // 2nnn CALL nnn Вызов подпрограммы по адресу nnn
        asmTextString.append(QString("CALL 0x%1 \t ; Call sub-routine from 0x%1 address").arg( NNN,0,16 ));
        m_stack.push_front( PC );
        PC = NNN;
        PC-=2; // correct call and jump pointer
        break;
    case 0x3: // 3xkk SE Vx, kk Пропустить следующую инструкцию, если регистр Vx = kk
        asmTextString.append(QString("SE V%1, 0x%2 \t ; Skip next instruction if V%1 = 0x%2").arg( X, 0, 16 ).arg( KK,0,16 ));
        if ( getRegister( X ) == KK)
        {
            PC+=2;
        }
        break;
    case 0x4: // 4xkk SNE Vx, kk Пропустить следующую инструкцию, если регистр Vx != kk
        asmTextString.append(QString("SNE V%1, 0x%2 \t ; Skip next instruction if V%1 != 0x%2").arg( X, 0, 16 ).arg( KK, 0, 16 ));
        if ( getRegister( X ) != KK)
        {
            PC+=2;
        }
        break;
    case 0x5: // 5xy0 SE Vx, Vy Пропустить следующую инструкцию, если Vx = Vy
        if ( LO == 0x0)
        {
            asmTextString.append(QString("SE V%1, V%2 \t ; Skip next instruction if V%1 = V%2").arg( X,0,16 ).arg( Y,0,16 ));
            if ( getRegister( X ) == getRegister( Y ))
            {
                PC+=2;
            }
        }else
        {
            asmTextString.append( QString("opCode 0x%1 is invalid").arg( opCode, 0, 16 ));
        }
        break;
    case 0x6: // 6xkk LD Vx, kk Загрузить в регистр Vx число kk, т.е. Vx = kk
        asmTextString.append(QString("LD V%1, 0x%2 \t ; Load 0x%2 to register V%1 ").arg( X, 0, 16 ).arg( KK, 0, 16 ));
        setRegister( X, KK );
        break;
    case 0x7: // 7xkk ADD Vx, kk Установить Vx = Vx + kk
        asmTextString.append(QString("ADD V%1, 0x%2 \t ; V%1 = V%1 + 0x%2").arg( X,0,16 ).arg( KK, 0, 16 ));
        setRegister( X, getRegister( X ) + KK );
        break;
    case 0x8:
        switch ( LO )
        {
        case 0x0:
            // 8xy0 LD Vx, Vy Установить Vx = Vy
            asmTextString.append(QString("LD V%1, V%2 \t ; Set register V%1 = V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
            setRegister( X, getRegister( Y ) );
            break;
        case 0x1:
            /**
                 * 8xy1 OR Vx, Vy Выполнить операцию дизъюнкция (логическое “ИЛИ”) над значениями регистров Vx и Vy,
                 * результат сохранить в Vx. Т.е. Vx = Vx | Vy
                 **/
            asmTextString.append(QString("OR V%1, V%2 \t ; Set register V%1 = V%1 | V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
            setRegister( X, ( getRegister( X ) | getRegister( Y ) ) );
            break;
        case 0x2:
            /**
                 * 8xy2 AND Vx, Vy Выполнить операцию конъюнкция (логическое “И”) над значениями регистров Vx и Vy,
                 * результат сохранить в Vx. Т.е. Vx = Vx & Vy
                 **/
            asmTextString.append(QString("OR V%1, V%2 \t ; Set register V%1 = V%1 & V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
            setRegister( X, ( getRegister( X ) & getRegister( Y ) ) );
            break;
        case 0x3:
            /**
                 * 8xy3 XOR Vx, Vy Выполнить операцию “исключающее ИЛИ” над значениями регистров Vx и Vy,
                 * результат сохранить в Vx. Т.е. Vx = Vx ^ Vy
                 **/
            asmTextString.append(QString("XOR V%1, V%2 \t ; Set register V%1 = V%1 ^ V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
            setRegister( X, ( getRegister( X ) ^ getRegister( Y ) ) );
            break;
        case 0x4:
            /**
                 * 8xy4 ADD Vx, Vy Значения Vx и Vy суммируются. Если результат больше, чем 8 бит (т.е.> 255)
                 * VF устанавливается в 1, иначе 0. Только младшие 8 бит результата сохраняются в Vx. Т.е. Vx = Vx + Vy
                 **/
            asmTextString.append(QString("ADD V%1, V%2 \t ; Set register V%1 = V%1 + V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
            setRegister( X, getSumCF( X ,Y ) );
            break;
        case 0x5: // 8xy5 SUB Vx, Vy Если Vx >= Vy, то VF устанавливается в 1, иначе 0. Затем Vy вычитается из Vx,
            // а результат сохраняется в Vx. Т.е. Vx = Vx - Vy
            asmTextString.append(QString("SUB V%1, V%2 \t ; Set register V%1 = V%1 - V%2").arg( X,0,16 ).arg( Y, 0, 16 ));
            if ( getRegister( X ) >= getRegister( Y ) )
            {
                setRegister( REG_VF ,0x1);
            }
            else
            {
                setRegister( REG_VF ,0x0);
            }
            setRegister( X, getRegister( X ) - getRegister( Y ) );
            break;
        case 0x6:
            /** 8xy6 SHR Vx {, Vy} Операция сдвига вправо на 1 бит. Сдвигается регистр Vx. Т.е. Vx = Vx >> 1.
                 *  До операции сдвига выполняется следующее: если младший бит (самый правый) регистра Vx равен 1, то VF = 1, иначе VF = 0
                 **/
            asmTextString.append(QString("SHR V%1 {, V%2} \t ; Set register V%1 = V%1 >> 1 ").arg(X,0,16 ).arg( Y, 0, 16 ));
            setRegister(REG_VF, ( ( getRegister( X ) >> 7) & 0x1) ) ;
            setRegister( X, ( getRegister( X ) >> 1 ) );
            break;
        case 0x7:
            /**
                 * 8xy7 SUBN Vx, Vy Если Vy >= Vx, то VF устанавливается в 1, иначе 0. Тогда Vx вычитается из Vy,
                 * и результат сохраняется в Vx. Т.е. Vx = Vy - Vx
                 **/
            asmTextString.append(QString("SUBN V%1, V%2 \t ; IF V%2 > V%1 { SET VF = 1} ELSE { SET VF = 0 } set register V%1 = V%2 - V%1 } ").arg( X,0,16 ).arg( Y, 0, 16 ));
            if ( getRegister( Y ) >= getRegister( X ) )
            {
                setRegister( REG_VF ,0x1);
            }
            else
            {
                setRegister( REG_VF ,0x0);
            }
            setRegister( X, getRegister( Y ) - getRegister( X ) );
            break;
        case 0xE:
            /**
                 * 8xyE SHL Vx {, Vy} Операция сдвига влево на 1 бит. Сдвигается регистр Vx. Т.е. Vx = Vx << 1.
                 *  До операции сдвига выполняется следующее: если младший бит (самый правый) регистра Vx равен 1, то VF = 1, иначе VF = 0
                 **/
            asmTextString.append(QString("SHL V%1 {, V%2} \t ; Set register V%1 = V%1 << 1 ").arg( X,0,16 ).arg( Y, 0, 16 ));
            setRegister(REG_VF, ( ( getRegister( X ) >> 7) & 0x1) ) ;
            setRegister( X, ( getRegister( X ) << 1 ) );
            break;
        default:
            break;
        }
        break;
    case 0x9: // 9xy0 SNE Vx, Vy Пропустить следующую инструкцию, если Vx != Vy
        if ( LO == 0x0)
        {
            asmTextString.append(QString("SNE V%1, V%2 \t ; Skip next instruction if V%1 != V%2").arg( X,0,16 ).arg( Y, 0, 16));
            if (getRegister( X) != getRegister( Y ))
            {
                PC+=2;
            }
        }else {
            asmTextString.append( QString("opCode 0x%1 is invalid").arg( opCode, 0, 16 ));
        }
        break;
    case 0xA:
        // Annn LD I, nnn Значение регистра I устанавливается в nnn
        asmTextString.append(QString("LD I, 0x%1 \t ; set register I to 0x%1").arg( NNN,0,16 ));
        setRegI( NNN );
        break;
    case 0xB:
        // Bnnn JP V0, nnn Перейти по адресу nnn + значение в регистре V0.
        asmTextString.append(QString("JP V0, 0x%1 \t ; Jump to address V0 + %1").arg( NNN,0,16 ));
        PC = getRegister( 0 ) + NNN;
        break;
    case 0xC: //Cxkk RND Vx, kk Устанавливается Vx = (случайное число от 0 до 255) & kk
        asmTextString.append(QString("RND V%1, 0x%2 \t ; Set register V%1 = random {0,255} & 0x%2 ").arg( X,0,16 ).arg( KK,0,16 ));
        setRegister(X, QRandomGenerator::global()->bounded( 256 ) & KK );
        break;
    case 0xD: /** Dxyn DRW Vx, Vy, n Нарисовать на экране спрайт. Эта инструкция считывает n байт по адресу
                    * содержащемуся в регистре I и рисует их на экране в виде спрайта c координатой Vx, Vy.
                    * Спрайты рисуются на экран по методу операции XOR, то есть если в том месте где мы рисуем спрайт
                    * уже есть нарисованные пиксели - они стираются, если их нет - рисуются. Если хоть один пиксель был стерт,
                    * то VF устанавливается в 1, иначе в 0.
                    **/
        asmTextString.append(QString("DRW V%1, V%3, 0x%5 \t ; Draw sprite (0x%5 bytes) in the pos saved V%1[0x%2], V%3[0x%4] ")
                                    .arg( X,0,16 ).arg( getRegister( X ),0,16 )
                                    .arg( Y,0,16 ).arg( getRegister( Y ),0,16 )
                                    .arg( LO,0,16 ));
        drawSprite( getRegister( X ), getRegister ( Y ), LO);
        break;
    case 0xE:
        if ( KK == 0x9E)
        {
            // Ex9E SKP Vx Пропустить следующую команду если клавиша, номер которой хранится в регистре Vx, нажата
            asmTextString.append(QString("SKP V%1 \t ; Skip next instruction if key number (save register V%1) pressed ").arg( X,0,16 ) );
            //qDebug() << "SKP V" << getRegister( X ) << " ; Skip next instruction if key number (save register V" << getRegister( X )<< ") pressed";
            if (m_keys.at( getRealKey ( getRegister( X ) )))
            {
                PC+=2;
            }
        }
        if ( KK == 0xA1)
        {
            // ExA1 SKNP Vx Пропустить следующую команду если клавиша, номер которой хранится в регистре Vx, не нажата
            asmTextString.append(QString("SKNP V%1 \t ; Skip next instruction if key number (save register V%1) not pressed ").arg( X,0,16 ) );
            //qDebug() << "SKNP V" << getRegister( X ) << " ; Skip next instruction if key number (save register V" << getRegister( X )<< ") not pressed";
            if (!m_keys.at( getRealKey ( getRegister( X ) )))
            {
                PC+=2;
            }
        }
        break;
    case 0xF:
        switch ( KK ) {
        case 0x07:// Fx07 LD Vx, DT Скопировать значение таймера задержки в регистр Vx
            asmTextString.append(QString("LD V%1, DT \t ; Set register V%1 = DATA_TIMER ").arg( X,0,16 ) );
            setRegister(X, delay_timer);
            break;
        case 0x0A:
            /**
                 * Fx0A LD Vx, K Ждать нажатия любой клавиши.
                 * Как только клавиша будет нажата записать ее номер в регистр Vx и перейти к выполнению следующей инструкции.
                 */
            asmTextString.append(QString("LD V%1, K \t ; Wait any key pressed, after SET register V%1 = KEY_NUMBER ").arg( X,0,16 ) );
            PC-=2;
            for (quint8 i=0; i < 16; ++i)
            {
                if (m_keys.testBit(i))
                {
                    setRegister( X, getRealKey(i) );
                    PC+=2;
                    break;
                }
            }

            break;
        case 0x15: // Fx15 LD DT, Vx Установить значение таймера задержки равным значению регистра Vx
            asmTextString.append(QString("LD DT, V%1 \t ; Set register DATA_TIMER = V%1 ").arg( X,0,16 ) );
            delay_timer = getRegister( X );
            break;
        case 0x18:// Fx18 LD ST, Vx Установить значение звукового таймера равным значению регистра Vx
            asmTextString.append(QString("LD ST, V%1 \t ; Set register SOUND_TIMER = V%1 ").arg( X,0,16 ) );
            sound_timer = getRegister( X );
            break;
        case 0x1E:
            // Fx1E ADD I, Vx Сложить значения регистров I и Vx, результат сохранить в I. Т.е. I = I + Vx
            asmTextString.append(QString("ADD I, V%1 \t ; Set register I = I + V%1 ").arg( X,0,16 ) );
            setRegI( getRegI() + getRegister( X ) );
            break;
        case 0x29:
            // Fx29 LD F, Vx Используется для вывода на экран символов встроенного шрифта размером 4x5 пикселей.
            // Команда загружает в регистр I адрес спрайта, значение которого находится в Vx.
            // Например, нам надо вывести на экран цифру 5. Для этого загружаем в Vx число 5.
            // Потом команда LD F, Vx загрузит адрес спрайта, содержащего цифру 5, в регистр I
            asmTextString.append(QString("LD F, V%1 \t ; Show sprite font ").arg( X,0,16 ) );
            setRegI( getRegister( X ) * SMALL_FONT_SIZE );
            break;
        case 0x30:
            // Fx30 LD HF, Vx Работает подобно команде Fx29, только загружает спрайты размером 8x10 пикселей
            asmTextString.append(QString("LD HF, V%1 \t ; Show sprite font 8x10 pixel ").arg( X,0,16 ) );
            setRegI( (getRegister( X ) * BIG_FONT_SIZE ) + BIG_FONT_OFFSET );
            break;
        case 0x33: // Fx33 LD B, Vx Сохранить значение регистра Vx в двоично-десятичном (BCD) представлении по адресам I, I+1 и I+2
            asmTextString.append(QString("LD B, V%1 \t ; Save register V%1 in memory {binary-decimal presentation},  address register I, I+1, I+2 ").arg( X,0,16 ) );
            saveBCDRegToI( getRegister ( X ) );
            break;
        case 0x55: // Fx55 LD [I], Vx Сохранить значения регистров от V0 до Vx в памяти, начиная с адреса находящегося в I
            asmTextString.append(QString("LD [I], V%1 \t ; Save registers {V0, V%1} in memory, start address = register I ").arg( X,0,16 ) );
            saveRegToMemory( X );
            break;
        case 0x65: // Fx65 LD Vx, [I] Загрузить значения регистров от V0 до Vx из памяти, начиная с адреса находящегося в I
            asmTextString.append(QString("LD V%1, [I] \t ; Load registers {V0, V%1} from memory, start address = register I ").arg( X,0,16 ) );
            loadRegFromMemory( X );
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

    qDebug() << QString("0x%1: %2").arg(PC,0,16).arg(asmTextString);
    emit showDecodeOpCode ( QString("0x%1:\t%2\t%3").arg(PC,0,16).arg(opCode,0,16).arg(asmTextString) );

    PC+=2;
    return;
}

void Chip8Emu::decreaseTimers()
{
    if (delay_timer > 0)
    {
        --delay_timer;
    }
}

void Chip8Emu::setRegister(quint8 m_reg, quint8 m_value)
{
    if (m_reg < 16)
    {
        m_regs[m_reg] = m_value ;
    }

}

quint16 Chip8Emu::getRegister(quint8 m_reg)
{
    if (m_reg <= REG_VF)
    {
        return m_regs.at(m_reg) ;
    }
    return 0x0;
}

void Chip8Emu::setRegI(quint16 m_value)
{
    regI = m_value;
}

quint16 Chip8Emu::getRegI()
{
    return regI;
}

quint16 Chip8Emu::getIndex(quint8 x, quint8 y)
{
    quint16 val = x + (y*DISPLAY_X);
    return ( val > MAX_DISPLAY_SIZE ) ? MAX_DISPLAY_SIZE : val;
}

// -- Draw function
void Chip8Emu::moveDown(quint8 m_line)
{
    qDebug() << "move down: " << m_line;
}

void Chip8Emu::moveRight()
{
    qDebug() << "move right, mode:" << m_ExtendedMode;
}

void Chip8Emu::moveLeft()
{
    qDebug() << "move left, mode:" << m_ExtendedMode;
}

void Chip8Emu::drawSprite(quint8 vx, quint8 vy, quint8 n)
{
    bool newPixel;
    bool existPixel;
    quint8 maxLine;
    quint8 drw;
    quint16 idx=0;

    maxLine = (( 0 == n ) | ( n > 16 ) ) ? 16 : n ; // check how many rows draw.

    if (!m_ExtendedMode)
    {        
        for (int row = 0; row < maxLine; ++row)
        {
            drw = m_memory.at(regI+row);
            for (int col = 0; col < 8; ++col)
            {
                newPixel = drw  & (1 << (7 - col));
                idx = getIndex ( (vx + col), (vy + row ));

                qDebug()<<"vx:"<<vx<<" vy:"<<vy<<"row:"<<row<<" col:"<<col<<" idx:" << idx;

                existPixel = m_screen.testBit( idx );
                if ( existPixel )
                {
                    setRegister( REG_VF, 0x1);
                }
                m_screen.setBit( idx, (existPixel ^ newPixel) );
            }
        }
    }
    else
    {
    }

}

void Chip8Emu::initDevice()
{
    PC = START_ADDR;               // set mem offset counter
    regI = 0;
    delay_timer = 0;               // clear delay timer;
    sound_timer = 0;               // clear sound timer;
    opcode_count = 0 ;
    m_memory.fill(0x0,RAM_SIZE);   // clear 4k ram memory
    m_regs.fill(0x0,16);
    m_stack.clear();
    m_screen.fill(false, DISPLAY_X * DISPLAY_Y);
    m_keys.fill(false, KEY_PAD);   // All keys unPressed
    m_ExtendedMode = false;
    m_ElapsedTime = 0;

    m_smallFont.append(0xF0).append(0x90).append(0x90).append(0x90).append(0xF0); 	// 0
    m_smallFont.append(0x20).append(0x60).append(0x20).append(0x20).append(0x70);	// 1
    m_smallFont.append(0xF0).append(0x10).append(0xF0).append(0x80).append(0xF0);	// 2
    m_smallFont.append(0xF0).append(0x10).append(0xF0).append(0x10).append(0xF0);	// 3
    m_smallFont.append(0x90).append(0x90).append(0xF0).append(0x10).append(0x10);	// 4
    m_smallFont.append(0xF0).append(0x80).append(0xF0).append(0x10).append(0xF0);	// 5
    m_smallFont.append(0xF0).append(0x80).append(0xF0).append(0x90).append(0xF0);	// 6
    m_smallFont.append(0xF0).append(0x10).append(0x20).append(0x40).append(0x40);	// 7
    m_smallFont.append(0xF0).append(0x90).append(0xF0).append(0x90).append(0xF0);	// 8
    m_smallFont.append(0xF0).append(0x90).append(0xF0).append(0x10).append(0xF0);	// 9
    m_smallFont.append(0xF0).append(0x90).append(0xF0).append(0x90).append(0x90);	// A
    m_smallFont.append(0xE0).append(0x90).append(0xE0).append(0x90).append(0xE0);	// B
    m_smallFont.append(0xF0).append(0x80).append(0x80).append(0x80).append(0xF0);	// C
    m_smallFont.append(0xE0).append(0x90).append(0x90).append(0x90).append(0xE0);	// D
    m_smallFont.append(0xF0).append(0x80).append(0xF0).append(0x80).append(0xF0);	// E
    m_smallFont.append(0xF0).append(0x80).append(0xF0).append(0x80).append(0x80);	// F

    // Load font
    m_memory.insert(0,m_smallFont);

}

void Chip8Emu::changeKeyState(quint8 key, bool state)
{
    m_keys.setBit(key, state);
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

quint8 Chip8Emu::getSumCF( quint8 x, quint8 y)
{
    quint16 val = x + y;
    if (val > 255)
    {
        m_regs[REG_VF] = 0x1 ;
    }
    else
    {
        m_regs[REG_VF] = 0x1 ;
    }
    return (val & 0x00FF);
}

void Chip8Emu::saveBCDRegToI(quint8 m_reg_val)
{    
    quint16 val_i   = getRegI();
    qDebug() <<"REG value:" <<m_reg_val<< " REG_I:" << val_i;

    m_memory[val_i] = (m_reg_val - (m_reg_val % 100)) / 100;
    m_reg_val -= m_memory.at(val_i) * 100;
    ++val_i;

    m_memory[val_i] = (m_reg_val - (m_reg_val % 10)) / 10;
    m_reg_val -= m_memory.at(val_i) * 10;
    ++val_i;

    m_memory[val_i] = m_reg_val;

}

void Chip8Emu::saveRegToMemory(quint8 m_reg_val)
{
    if (m_reg_val <= REG_VF)
    {
        quint8 idx = getRegI();
        for (int i=0; i<=m_reg_val; ++i)
        {
            m_memory[idx + i] = getRegister( i );
        }
    }
}

void Chip8Emu::loadRegFromMemory(quint8 m_reg_val)
{
    if (m_reg_val <= REG_VF)
    {
        quint8 idx = getRegI();
        for (int i=0; i<=m_reg_val; ++i)
        {
            setRegister(i, m_memory.at(idx + i) );
        }
    }
}

quint8 Chip8Emu::getRealKey (quint8 m_emu_key)
{
    /**   Real           Emu        ScanCode
    * +-+-+-+-+   | +-+-+-+-+   | +-+-+-+-+
    * |1|2|3|C|   | |1|2|3|4|   | |1|2|3|12|
    * +-+-+-+-+   | +-+-+-+-+   | +-+-+-+-+
    * |4|5|6|D|   | |Q|W|E|R|   | |4|5|6|13|
    * +-+-+-+-+   | +-+-+-+-+   | +-+-+-+-+
    * |7|8|9|E|   | |A|S|D|F|   | |7|8|9|14|
    * +-+-+-+-+   | +-+-+-+-+   | +-+-+-+-+
    * |A|0|B|F|   | |Z|X|C|V|   | |10|0|11|15|
    * +-+-+-+-+   | +-+-+-+-+   | +-+-+-+-+
    **/

    quint8 value;
    switch (m_emu_key)
    {
    case 0:  value = 1; break;
    case 1:  value = 2; break;
    case 2:  value = 3; break;
    case 3:  value = 12; break;
    case 4:  value = 4; break;
    case 5:  value = 5; break;
    case 6:  value = 6; break;
    case 7:  value = 13; break;
    case 8:  value = 7; break;
    case 9:  value = 8; break;
    case 10: value = 9; break;
    case 11: value = 14; break;
    case 12: value = 10; break;
    case 13: value = 0; break;
    case 14: value = 11; break;
    case 15: value = 15; break;
    default:
        value = 0; break;
        break;
    }
    //qDebug() << "Real key code:" << value;
    return value;
}














