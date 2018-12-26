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
#include <QCheckBox>
#include <QByteArray>
#include <QDataStream>
#include <QDataWidgetMapper>
#include <QAbstractListModel>
#include "mydefs.h"


class CPUBoxWidget : public QWidget
{
    Q_OBJECT
public:    
    explicit CPUBoxWidget( QWidget *parent = Q_NULLPTR);
    void setModel(QAbstractListModel *m_model);

public slots:
    void updateRegValues(QByteArray msg);

private:    
    void showValues(bool flag);

    QLineEdit* v0LED;
    QLineEdit* v1LED;
    QLineEdit* v2LED;
    QLineEdit* v3LED;
    QLineEdit* v4LED;
    QLineEdit* v5LED;
    QLineEdit* v6LED;
    QLineEdit* v7LED;
    QLineEdit* v8LED;
    QLineEdit* v9LED;
    QLineEdit* vaLED;
    QLineEdit* vbLED;
    QLineEdit* vcLED;
    QLineEdit* vdLED;
    QLineEdit* veLED;
    QLineEdit* vfLED;

    QLineEdit* ILED;
    QLineEdit* ILED_0;
    QLineEdit* ILED_1;
    QLineEdit* ILED_2;

    QLineEdit* PCLED;
    QLineEdit* delayTimerLED;
    QLineEdit* soundTimerLED;
    QTextEdit* stackTEdit;    
    QVector<quint8> m_regs;      // 16 registers 8bit size;
    QVector<quint16> m_stack;     // deep 16 levels;
    quint16 PC;             // mem offset counter
    quint16 regI;           // 16bit address register I
    quint8 regI_0;         // 16bit address Memory [ register I ]
    quint8 regI_1;         // 16bit address Memory [ register I+1 ]
    quint8 regI_2;         // 16bit address Memory [ register I+2 ]
    quint8 delay_timer;     // 8bit delay timer;
    quint8 sound_timer;     // 8bit sound timer;

    QDataWidgetMapper *mapper;

};

#endif // CPUBOXWIDGET_H
