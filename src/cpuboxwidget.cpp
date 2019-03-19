#include "cpuboxwidget.h"

//TODO need change recived data from message to DataModel

CPUBoxWidget::CPUBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    /**
     * @brief Widget's layout:
     * +-----------------------------------------------------------------------+
     * +                            mainHBoxLayout                             +
     * +-----------------------------+===========+-----------------------------+
     * +        leftVBoxLayout       +           +        rightVBoxLayout      +
     * +-----------------------------+===========+-----------------------------+
     * +           QLabel            +           +           QLabel            +
     * +-----------------------------+           +-----------------------------+
     * +         QSpaceItem          +           +         QSpaceItem          +
     * +-----------------------------+           +-----------------------------+
     * +        QGridLayout          +
     * +   c0  +   c1  +  c2  +  c4  +
     * +   c0  +   c1  +  c2  +  c4  +
     * +   c0  +   c1  +  c2  +  c4  +
     * +   c0  +   c1  +  c2  +  c4  +
     * +-----------------------------+===========+-----------------------------+
     */

    QGridLayout *registersGrid = new QGridLayout();
    {
        registersGrid->addWidget(new QLabel( tr("v0") ), 0, 0 );

        v0LED = new QLineEdit();
        v0LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v0LED->setReadOnly( true );
        registersGrid->addWidget(v0LED, 0, 1 );

        registersGrid->addWidget(new QLabel( tr("v8") ), 0, 2 );

        v8LED = new QLineEdit();
        v8LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v8LED->setReadOnly( true );
        registersGrid->addWidget(v8LED, 0, 3 );

        registersGrid->addWidget(new QLabel( tr("v1") ), 1, 0 );

        v1LED = new QLineEdit();
        v1LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v1LED->setReadOnly( true );
        registersGrid->addWidget(v1LED, 1, 1 );

        registersGrid->addWidget(new QLabel( tr("v9") ), 1, 2 );

        v9LED = new QLineEdit();
        v9LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v9LED->setReadOnly( true );
        registersGrid->addWidget(v9LED, 1, 3 );

        registersGrid->addWidget(new QLabel( tr("v2") ), 2, 0 );

        v2LED = new QLineEdit();
        v2LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v2LED->setReadOnly( true );
        registersGrid->addWidget(v2LED, 2, 1 );

        registersGrid->addWidget(new QLabel( tr("vA") ), 2, 2 );

        vALED = new QLineEdit();
        vALED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        vALED->setReadOnly( true );
        registersGrid->addWidget(vALED, 2, 3 );

        registersGrid->addWidget(new QLabel( tr("v3") ), 3, 0 );

        v3LED = new QLineEdit();
        v3LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v3LED->setReadOnly( true );
        registersGrid->addWidget(v3LED, 3, 1 );

        registersGrid->addWidget(new QLabel( tr("vB") ), 3, 2 );

        vBLED = new QLineEdit();
        vBLED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        vBLED->setReadOnly( true );
        registersGrid->addWidget(vBLED, 3, 3 );

        registersGrid->addWidget(new QLabel( tr("v4") ), 4, 0 );

        v4LED = new QLineEdit();
        v4LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v4LED->setReadOnly( true );
        registersGrid->addWidget(v4LED, 4, 1 );

        registersGrid->addWidget(new QLabel( tr("vC") ), 4, 2 );

        vCLED = new QLineEdit();
        vCLED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        vCLED->setReadOnly( true );
        registersGrid->addWidget(vCLED, 4, 3 );

        registersGrid->addWidget(new QLabel( tr("v5") ), 5, 0 );

        v5LED = new QLineEdit();
        v5LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v5LED->setReadOnly( true );
        registersGrid->addWidget(v5LED, 5, 1 );

        registersGrid->addWidget(new QLabel( tr("vD") ), 5, 2 );

        vDLED = new QLineEdit();
        vDLED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        vDLED->setReadOnly( true );
        registersGrid->addWidget(vDLED, 5, 3 );

        registersGrid->addWidget(new QLabel( tr("v6") ), 6, 0 );

        v6LED = new QLineEdit();
        v6LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v6LED->setReadOnly( true );
        registersGrid->addWidget(v6LED, 6, 1 );

        registersGrid->addWidget(new QLabel( tr("vE") ), 6, 2 );

        vELED = new QLineEdit();
        vELED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        vELED->setReadOnly( true );
        registersGrid->addWidget(vELED, 6, 3 );

        registersGrid->addWidget(new QLabel( tr("v7") ), 7, 0 );

        v7LED = new QLineEdit();
        v7LED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        v7LED->setReadOnly( true );
        registersGrid->addWidget(v7LED, 7, 1 );

        registersGrid->addWidget(new QLabel( tr("vF") ), 7, 2 );

        vFLED = new QLineEdit();
        vFLED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        vFLED->setReadOnly( true );
        registersGrid->addWidget(vFLED, 7, 3 );

        QLabel *delayLabel = new QLabel( tr("Delay timer:"));
        delayLabel->setAlignment( Qt::AlignLeft |
                                  Qt::AlignVCenter );
        registersGrid->addWidget(delayLabel , 8 , 1);

        delayTimerLED = new QLineEdit( ("0x0000") );
        delayTimerLED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        delayTimerLED->setReadOnly( false );
        registersGrid->addWidget(delayTimerLED , 8 , 3);

        QLabel *soundLabel = new QLabel( tr("Sound timer:"));
        soundLabel->setAlignment( Qt::AlignLeft |
                                  Qt::AlignVCenter );
        registersGrid->addWidget(soundLabel , 9 , 1);

        soundTimerLED = new QLineEdit( ("0x0000") );
        soundTimerLED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        soundTimerLED->setReadOnly( false );
        registersGrid->addWidget(soundTimerLED , 9, 3);
    }

    QLabel *lTitleLabel = new QLabel(tr("CPU Registers"));
    lTitleLabel->setAlignment( Qt::AlignCenter );

    QVBoxLayout *leftVBoxLayout = new QVBoxLayout();
    leftVBoxLayout->addWidget(lTitleLabel);
    leftVBoxLayout->addLayout(registersGrid);
    leftVBoxLayout->addSpacerItem( new QSpacerItem(20,40,QSizePolicy::Expanding,
                                                   QSizePolicy::Expanding) );

    //--------------------------------------------------------------------------

    QLabel *rTitleLabel = new QLabel(tr("Other registers"));
    rTitleLabel->setAlignment( Qt::AlignCenter );

    QFormLayout *indexAndTimersGrid = new QFormLayout();
    {
        QLabel *indexLabel = new QLabel( tr("I Register:"));
        indexLabel->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

        ILED = new QLineEdit( ("0x0000") );
        ILED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        ILED->setReadOnly( true );

        QLabel *pcLabel = new QLabel( tr("PC:"));
        pcLabel->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

        PCLED = new QLineEdit( ("0x0000") );
        PCLED->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        PCLED->setReadOnly( false );

        QLabel *stackLabel = new QLabel( tr("Call Stack"));
        stackLabel->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

         callStack = new QListWidget();
        QFontMetrics *fontMetrix = new QFontMetrics( callStack->font() );
        callStack->setFixedWidth( fontMetrix->width(QString("0x0000")) * 2 );

        indexAndTimersGrid->addRow(indexLabel, ILED);
        indexAndTimersGrid->addRow(pcLabel, PCLED);
        indexAndTimersGrid->addRow(stackLabel, callStack);
        //        indexAndTimersGrid->addWidget(indexLabel,      0, 0 );
        //        indexAndTimersGrid->addWidget(ILED,            0, 1 );
        //        indexAndTimersGrid->addWidget(pcLabel,         1, 0 );
        //        indexAndTimersGrid->addWidget(PCLED,           1, 1 );
        //        indexAndTimersGrid->addWidget(stackLabel,      2, 0 );
        //        indexAndTimersGrid->addWidget(callStack,       2, 1 );
    }

    QVBoxLayout *rightVBoxLayout = new QVBoxLayout();
    rightVBoxLayout->addWidget( rTitleLabel );
    rightVBoxLayout->addLayout(indexAndTimersGrid);

    QHBoxLayout *mainHBoxLayout = new QHBoxLayout();
    mainHBoxLayout->addLayout(leftVBoxLayout);
    mainHBoxLayout->addSpacerItem( new QSpacerItem(20,20,
                                                   QSizePolicy::MinimumExpanding,
                                                   QSizePolicy::MinimumExpanding) );
    mainHBoxLayout->addLayout(rightVBoxLayout);

    this->setLayout(mainHBoxLayout);
}

void CPUBoxWidget::registerIChanged(quint16 value)
{
    ILED->setText( QString("0x%1").arg(value,0,16) );
}

void CPUBoxWidget::pointerCodeChanged(quint16 value)
{
    PCLED->setText( QString("0x%1").arg(value,0,16) );
}

void CPUBoxWidget::delayTimerChanged(quint8 value)
{
    delayTimerLED->setText( QString("0x%1").arg(value,0,16) );
}

void CPUBoxWidget::soundTimerChanged(quint8 value)
{
    soundTimerLED->setText( QString("0x%1").arg( value,0,16) );
}

void CPUBoxWidget::registerValueChanged(quint8 m_reg, quint8 value)
{
    switch (m_reg) {
    case 0x0:
        v0LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x1:
        v1LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x2:
        v2LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x3:
        v3LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x4:
        v4LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x5:
        v5LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x6:
        v6LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x7:
        v7LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x8:
        v8LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0x9:
        v9LED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0xA:
        vALED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0xB:
        vBLED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0xC:
        vCLED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0xD:
        vDLED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0xE:
        vELED->setText( QString("0x%1").arg( value,0,16) );
        break;
    case 0xF:
        vFLED->setText( QString("0x%1").arg( value,0,16) );
        break;
    default:
        break;
    }
}

void CPUBoxWidget::stackPop()
{    
    callStack->takeItem( 0 );
}

void CPUBoxWidget::stackPush(quint16 value)
{
    callStack->insertItem(0,QString("0x%1").arg( value,0,16));
}



