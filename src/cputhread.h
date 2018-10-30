#ifndef CPUTHREAD_H
#define CPUTHREAD_H

#include <QObject>
#include <QBitArray>
#include <QByteArray>
#include <QElapsedTimer>
#include <QDebug>

#define RAM_SIZE 4096
#define START_ADDR 0x200
#define DISPLAY_X 64
#define DISPLAY_Y 32
#define DEBUG 1

class CPUThread : public QObject
{
    Q_OBJECT
    // Свойство, управляющее работой потока
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

    // Сообщение с обновлением экрана
    Q_PROPERTY(QBitArray screen READ getScreen NOTIFY screenChanged)

    // Свойство предназначенное для загрузки ROM в память эмулятора
    Q_PROPERTY(QByteArray memory READ getMemory WRITE setMemory )
    //Свойство предназначенное для выбора режима CHIp-8 SuperCHIP
    Q_PROPERTY(bool workMode READ getWorkMode WRITE setWorkMode )

    bool m_running;
    QBitArray m_screen;
    QByteArray m_memory;

    // Счётчик распозанных опткодов
    int opCodeCount;

public:
    explicit CPUThread(QObject *parent = nullptr);
    bool running() const;
    bool getWorkMode() const;
    QBitArray getScreen() const;
    QByteArray getMemory() const;

signals:
    void finished();    // Сигнал, по которому будем завершать поток, после завершения метода run
    void runningChanged(bool running);
    void screenChanged(QBitArray m_screen);

public slots:
    void run(); // Метод с полезной нагрузкой, который может выполняться в цикле
    void setRunning(bool running);
    void setMemory(QByteArray rom);
    void setWorkMode(bool mode);

private:
    unsigned short PC;   // mem offset counter
    unsigned short regI; // address register I
    short delay_timer;         // delay timer;
    short sound_timer;         // sound timer;
    bool m_WorkMode;

    void executeNextOpcode();
    void decreaseTimers();
    bool drawSprite(int vx,int vy, int n);

};

#endif // CPUTHREAD_H
