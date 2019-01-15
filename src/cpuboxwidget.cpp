#include "cpuboxwidget.h"


CPUBoxWidget::CPUBoxWidget(QWidget *parent)
    : QWidget(parent)
{    
    QVBoxLayout* mainVBoxLayout = new QVBoxLayout();

    QFormLayout *formLayout = new QFormLayout;
    {
        ILED = new QLineEdit( ("0x0") );
        ILED->setEnabled( false );

        MemoryCellLED_0 = new QLineEdit( ("0x0") );
        MemoryCellLED_0->setEnabled( false );

        MemoryCellLED_1 = new QLineEdit( ("0x0") );
        MemoryCellLED_1->setEnabled( false );

        MemoryCellLED_2 = new QLineEdit( ("0x0") );
        MemoryCellLED_2->setEnabled( false );

        delayTimerLED = new QLineEdit( ("0x0") );
        delayTimerLED->setEnabled( false );

        soundTimerLED = new QLineEdit( ("0x0") );
        soundTimerLED->setEnabled( false );

        PCLED = new QLineEdit( ("0x0") );
        PCLED->setEnabled( false );

        // TODO set width for editor
        stackTEdit = new QTextEdit();
        stackTEdit->setReadOnly( true );
        stackTEdit->setText(tr("0x000"));

        formLayout->addRow(tr("Register I:"), ILED);
        formLayout->addRow(tr("Mem[ reg I ]:"), MemoryCellLED_0);
        formLayout->addRow(tr("Mem[ reg I+1 ]:"), MemoryCellLED_1);
        formLayout->addRow(tr("Mem[ reg I+2 ]:"), MemoryCellLED_2);

        formLayout->addRow(tr("Delay timer:"), delayTimerLED);
        formLayout->addRow(tr("Sound timer:"), soundTimerLED);
        formLayout->addRow(tr("PC:"), PCLED);
        formLayout->addRow(tr("Stacks:"), stackTEdit);
    }

    QGroupBox* gb_Right = new QGroupBox();
    gb_Right->setTitle( tr("Common registers:") );
    gb_Right->setLayout( formLayout);

    registerTableView = new QTableView(this);
    registerTableView->setAutoScroll( false);

    QHBoxLayout* hbox_1 = new QHBoxLayout();
    hbox_1->addWidget( registerTableView );
    hbox_1->addSpacerItem( new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Expanding) );
    hbox_1->addWidget( gb_Right );

    mainVBoxLayout->addLayout( hbox_1 );

    this->setLayout(mainVBoxLayout);
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

void CPUBoxWidget::memoryCellChanged(quint16 value_0, quint16 value_1, quint16 value_2)
{
    MemoryCellLED_0->setText( QString("0x%1").arg( value_0,0,16) );
    MemoryCellLED_1->setText( QString("0x%1").arg( value_1,0,16) );
    MemoryCellLED_2->setText( QString("0x%1").arg( value_2,0,16) );
}



