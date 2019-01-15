#ifndef CPUBOXWIDGET_H
#define CPUBOXWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include "mydefs.h"


class CPUBoxWidget : public QWidget
{
    Q_OBJECT
public:    
    explicit CPUBoxWidget( QWidget *parent = Q_NULLPTR);
    void setModel(QAbstractListModel *m_model);

public slots:
    void registerIChanged( quint16 value );
    void pointerCodeChanged( quint16 value);
    void delayTimerChanged( quint8 value );
    void soundTimerChanged( quint8 value );
//    void memoryCellChanged( quint16 value_0, quint16 value_1, quint16 value_2 );
    void registerValueChanged(quint8 m_reg ,quint8 value );

private:    
    QLineEdit *ILED;
//    QLineEdit *MemoryCellLED_0;
//    QLineEdit *MemoryCellLED_1;
//    QLineEdit *MemoryCellLED_2;

    QLineEdit *PCLED;
    QLineEdit *delayTimerLED;
    QLineEdit *soundTimerLED;

    QLineEdit *v0LED;
    QLineEdit *v1LED;
    QLineEdit *v2LED;
    QLineEdit *v3LED;
    QLineEdit *v4LED;
    QLineEdit *v5LED;
    QLineEdit *v6LED;
    QLineEdit *v7LED;

    QLineEdit *v8LED;
    QLineEdit *v9LED;
    QLineEdit *vALED;
    QLineEdit *vBLED;
    QLineEdit *vCLED;
    QLineEdit *vDLED;
    QLineEdit *vELED;
    QLineEdit *vFLED;

//    QTextEdit *stackTEdit;
    QListWidget *callStack;

    QVector<quint16> m_stack;     // deep 16 levels;
};

#endif // CPUBOXWIDGET_H
