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

class CPUBoxWidget : public QWidget
{
    Q_OBJECT
public:    
    explicit CPUBoxWidget( QWidget *parent = Q_NULLPTR);


private:
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
    QLineEdit* SPLED;
    QLineEdit* delayTimerLED;
    QLineEdit* soundTimerLED;
    QTextEdit* stackTEdit;
    QCheckBox* showAsBinaryCHB;
};

#endif // CPUBOXWIDGET_H
