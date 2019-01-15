#ifndef CPUBOXWIDGET_H
#define CPUBOXWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>
#include <QLineEdit>
#include <QTableView>
#include "mydefs.h"


class CPUBoxWidget : public QWidget
{
    Q_OBJECT
public:    
    explicit CPUBoxWidget( QWidget *parent = Q_NULLPTR);    

public slots:
    void registerIChanged( quint16 value );
    void pointerCodeChanged( quint16 value);
    void delayTimerChanged( quint8 value );
    void soundTimerChanged( quint8 value );
    void memoryCellChanged( quint16 value_0, quint16 value_1, quint16 value_2 );

private:    
    QLineEdit* ILED;
    QLineEdit* MemoryCellLED_0;
    QLineEdit* MemoryCellLED_1;
    QLineEdit* MemoryCellLED_2;

    QLineEdit* PCLED;
    QLineEdit* delayTimerLED;
    QLineEdit* soundTimerLED;
    QTextEdit* stackTEdit;        

    QVector<quint16> m_stack;     // deep 16 levels;

    QTableView *registerTableView;
};

#endif // CPUBOXWIDGET_H
