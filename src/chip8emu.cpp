#include <QDebug>
#include "chip8emu.h"

Chip8Emu::Chip8Emu(QObject *parent)
    : QObject(parent)
    , PC(START_ADDR)
{
    m_memory.fill(0x0,RAM_SIZE);   // clear 4k ram memory
}

void Chip8Emu::loadData2Memory(QByteArray &data)
{
    qDebug() << "load: " << data.size() << " bytes";
    if ( !data.isEmpty() &&
         ( data.size() <= RAM_SIZE - START_ADDR)  ) {
        m_memory.insert(START_ADDR,data);

        emit ReadyToWork(true);
    }

}
