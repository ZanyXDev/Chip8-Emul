#include "cpuboxwidget.h"

//TODO need change recived data from message to DataModel

CPUBoxWidget::CPUBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vbox_1 = new QVBoxLayout();

    QGridLayout * grLayout = new QGridLayout ();
    {
        // ----------------- Left rows
        grLayout->addWidget(new QLabel(tr("V0:")),0,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),0,1 );
        v0LED = new QLineEdit( ("0x0") );
        v0LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v0LED->setEnabled( false );


        grLayout->addWidget( v0LED,0,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),0,3 );

        grLayout->addWidget(new QLabel(tr("V1:")),1,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),1,1 );
        v1LED = new QLineEdit( ("0x0") );
        v1LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v1LED->setEnabled( false );
        grLayout->addWidget( v1LED,1,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),1,3 );

        grLayout->addWidget(new QLabel(tr("V2:")),2,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),2,1 );
        v2LED = new QLineEdit( ("0x0") );
        v2LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v2LED->setEnabled( false );
        grLayout->addWidget( v2LED,2,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),2,3 );

        grLayout->addWidget(new QLabel(tr("V3:")),3,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),3,1 );
        v3LED = new QLineEdit( ("0x0") );
        v3LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v3LED->setEnabled( false );
        grLayout->addWidget( v3LED,3,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),3,3 );

        grLayout->addWidget(new QLabel(tr("V4:")),4,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),4,1 );
        v4LED = new QLineEdit( ("0x0") );
        v4LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v4LED->setEnabled( false );
        grLayout->addWidget( v4LED,4,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),4,3 );

        grLayout->addWidget(new QLabel(tr("V5:")),5,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),5,1 );
        v5LED = new QLineEdit( ("0x0") );
        v5LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v5LED->setEnabled( false );
        grLayout->addWidget( v5LED,5,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),5,3 );

        grLayout->addWidget(new QLabel(tr("V6:")),6,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),6,1 );
        v6LED = new QLineEdit( ("0x0") );
        v6LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v6LED->setEnabled( false );
        grLayout->addWidget( v6LED,6,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),6,3 );

        grLayout->addWidget(new QLabel(tr("V7:")),7,0);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),7,1 );
        v7LED = new QLineEdit( ("0x0") );
        v7LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v7LED->setEnabled( false );
        grLayout->addWidget( v7LED,7,2 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),7,3 );

        // ---- Right rows
        grLayout->addWidget(new QLabel(tr("V8:")),0,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),0,5 );
        v8LED = new QLineEdit( ("0x0") );
        v8LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v8LED->setEnabled( false );
        grLayout->addWidget( v8LED,0,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),0,7 );

        grLayout->addWidget(new QLabel(tr("V9:")),1,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),1,5 );
        v9LED = new QLineEdit( ("0x0") );
        v9LED->setToolTip( "0.0.0.0.0.0.0.0" );
        v9LED->setEnabled( false );
        grLayout->addWidget( v9LED,1,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),1,7 );

        grLayout->addWidget(new QLabel(tr("Va:")),2,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),2,5 );
        vaLED = new QLineEdit( ("0x0") );
        vaLED->setToolTip( "0.0.0.0.0.0.0.0" );
        vaLED->setEnabled( false );
        grLayout->addWidget( vaLED,2,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),2,7 );

        grLayout->addWidget(new QLabel(tr("Vb:")),3,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),3,5 );
        vbLED = new QLineEdit( ("0x0") );
        vbLED->setToolTip( "0.0.0.0.0.0.0.0" );
        vbLED->setEnabled( false );
        grLayout->addWidget( vbLED,3,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),3,7 );

        grLayout->addWidget(new QLabel(tr("Vc:")),4,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),4,5 );
        vcLED = new QLineEdit( ("0x0") );
        vcLED->setToolTip( "0.0.0.0.0.0.0.0" );
        vcLED->setEnabled( false );
        grLayout->addWidget( vcLED,4,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),4,7 );

        grLayout->addWidget(new QLabel(tr("Vd:")),5,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),5,5 );
        vdLED = new QLineEdit( ("0x0") );
        vdLED->setToolTip( "0.0.0.0.0.0.0.0" );
        vdLED->setEnabled( false );
        grLayout->addWidget( vdLED,5,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),5,7 );

        grLayout->addWidget(new QLabel(tr("Ve:")),6,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),6,5 );
        veLED = new QLineEdit( ("0x0") );
        veLED->setToolTip( "0.0.0.0.0.0.0.0" );
        veLED->setEnabled( false );
        grLayout->addWidget( veLED,6,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),6,7 );

        grLayout->addWidget(new QLabel(tr("Vf:")),7,4);
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),7,5 );
        vfLED = new QLineEdit( ("0x0") );
        vfLED->setToolTip( "0.0.0.0.0.0.0.0" );
        vfLED->setEnabled( false );
        grLayout->addWidget( vfLED,7,6 );
        grLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored),7,7 );
    }
    QGroupBox* gb_Left = new QGroupBox();
    gb_Left->setTitle( tr("Data registers:") );
    gb_Left->setLayout( grLayout );

    QFormLayout *formLayout = new QFormLayout;
    {
        ILED = new QLineEdit( ("0x0") );
        ILED->setToolTip( "0.0.0.0.0.0.0.0" );
        ILED->setEnabled( false );

        ILED_0 = new QLineEdit( ("0x0") );
        ILED_0->setToolTip( "0.0.0.0.0.0.0.0" );
        ILED_0->setEnabled( false );

        ILED_1 = new QLineEdit( ("0x0") );
        ILED_1->setToolTip( "0.0.0.0.0.0.0.0" );
        ILED_1->setEnabled( false );

        ILED_2 = new QLineEdit( ("0x0") );
        ILED_2->setToolTip( "0.0.0.0.0.0.0.0" );
        ILED_2->setEnabled( false );

        delayTimerLED = new QLineEdit( ("0x0") );
        delayTimerLED->setToolTip( "0.0.0.0.0.0.0.0" );
        delayTimerLED->setEnabled( false );

        soundTimerLED = new QLineEdit( ("0x0") );
        soundTimerLED->setToolTip( "0.0.0.0.0.0.0.0" );
        soundTimerLED->setEnabled( false );

        PCLED = new QLineEdit( ("0x0") );
        PCLED->setToolTip( "0.0.0.0.0.0.0.0" );
        PCLED->setEnabled( false );

        // TODO set width for editor
        stackTEdit = new QTextEdit();
        stackTEdit->setReadOnly( true );
        stackTEdit->setText(tr("0x000"));

        formLayout->addRow(tr("Register I:"), ILED);
        formLayout->addRow(tr("Mem[ reg I]:"), ILED_0);
        formLayout->addRow(tr("Mem[ reg I+1]:"), ILED_1);
        formLayout->addRow(tr("Mem[ reg I+2]:"), ILED_2);

        formLayout->addRow(tr("Delay timer:"), delayTimerLED);
        formLayout->addRow(tr("Sound timer:"), soundTimerLED);
        formLayout->addRow(tr("PC:"), PCLED);
        formLayout->addRow(tr("Stacks:"), stackTEdit);
    }

    QGroupBox* gb_Right = new QGroupBox();
    gb_Right->setTitle( tr("Common registers:") );
    gb_Right->setLayout( formLayout);

    QHBoxLayout* hbox_1 = new QHBoxLayout();
    hbox_1->addWidget( gb_Left );
    hbox_1->addSpacerItem( new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Ignored) );
    hbox_1->addWidget( gb_Right );

    QFrame* hLine = new QFrame();
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);

    showAsBinaryCHB = new QCheckBox(tr("Show value's as binary"));
    // TODO add private slots for show register's value as binary or hexdecimal
    vbox_1->addLayout( hbox_1);
    vbox_1->addWidget( hLine );
    vbox_1->addWidget( showAsBinaryCHB );

    this->setLayout(vbox_1);
}

void CPUBoxWidget::updateRegValues(QByteArray msg)
{
    QDataStream in(&msg,QIODevice::ReadOnly);
    if (in.version() == QDataStream::Qt_5_10)
    {
        m_regs.fill(0x0,MAX_REG);
        m_stack.clear();

        in >> PC;
        in >> regI;
        in >> regI_0;
        in >> regI_1;
        in >> regI_2;
        in >> delay_timer;
        in >> sound_timer;
        in >> m_stack;
        in >> m_regs;

        //        in >> vector_size;
        //        m_stack.clear();

        //        for (int i=0;i<vector_size;i++)
        //        {
        //            in >> val_16;
        //            m_stack.append( val_16 );
        //        }

        //        in >> vector_size;
        //        m_regs.clear();
        //        for (int i=0;i<vector_size;i++)
        //        {
        //            in >> val_8;
        //            m_regs.append( val_8 );
        //        }

    }
    showValues( showAsBinaryCHB->isChecked() );
}

QString CPUBoxWidget::hexToBinaryString(quint8 value)
{
    QString result;
    return result;
}

void CPUBoxWidget::showValues(bool flag)
{
    QString txt;
    QString hint;
    // ------ parser --------
    if (flag)
    {

    }
    else
    {
        // FIXME Databinding ????
        v0LED->setText( QString("0x%1").arg( m_regs.at(0),0,16) );
        v0LED->setToolTip( hexToBinaryString( m_regs.at(0) ) );

        v1LED->setText( QString("0x%1").arg( m_regs.at(1),0,16) );
        v1LED->setToolTip( hexToBinaryString(m_regs.at(1)) );

        v2LED->setText( QString("0x%1").arg(m_regs.at(2),0,16) );
        v2LED->setToolTip( hexToBinaryString(m_regs.at(2)) );

        v3LED->setText( QString("0x%1").arg(m_regs.at(3),0,16) );
        v3LED->setToolTip( hexToBinaryString(m_regs.at(3)) );

        v4LED->setText( QString("0x%1").arg(m_regs.at(4),0,16) );
        v4LED->setToolTip( hexToBinaryString(m_regs.at(4)) );

        v5LED->setText( QString("0x%1").arg(m_regs.at(5),0,16) );
        v5LED->setToolTip( hexToBinaryString(m_regs.at(5)) );

        v6LED->setText( QString("0x%1").arg(m_regs.at(6),0,16) );
        v6LED->setToolTip( hexToBinaryString(m_regs.at(6)) );

        v7LED->setText( QString("0x%1").arg(m_regs.at(7),0,16) );
        v7LED->setToolTip( hexToBinaryString(m_regs.at(7)) );

        v8LED->setText( QString("0x%1").arg(m_regs.at(8),0,16) );
        v8LED->setToolTip( hexToBinaryString(m_regs.at(8)) );

        v9LED->setText( QString("0x%1").arg(m_regs.at(9),0,16) );
        v9LED->setToolTip( hexToBinaryString(m_regs.at(9)) );

        vaLED->setText( QString("0x%1").arg(m_regs.at(10),0,16) );
        vaLED->setToolTip( hexToBinaryString(m_regs.at(10)) );

        vbLED->setText( QString("0x%1").arg(m_regs.at(11),0,16) );
        vbLED->setToolTip( hexToBinaryString(m_regs.at(11)) );

        vcLED->setText( QString("0x%1").arg(m_regs.at(12),0,16) );
        vcLED->setToolTip( hexToBinaryString(m_regs.at(12)) );

        vdLED->setText( QString("0x%1").arg(m_regs.at(13),0,16) );
        vdLED->setToolTip( hexToBinaryString(m_regs.at(13)) );

        veLED->setText( QString("0x%1").arg(m_regs.at(14),0,16) );
        veLED->setToolTip( hexToBinaryString(m_regs.at(14)) );

        vfLED->setText( QString("0x%1").arg(m_regs.at(15),0,16) );
        vfLED->setToolTip( hexToBinaryString(m_regs.at(15)) );

        ILED->setText( QString("0x%1").arg(regI,0,16) );
        ILED->setToolTip( hexToBinaryString(regI) );

        PCLED->setText( QString("0x%1").arg(PC,0,16) );
        PCLED->setToolTip( hexToBinaryString(PC) );

        delayTimerLED->setText( QString("0x%1").arg(delay_timer,0,16) );
        delayTimerLED->setToolTip( hexToBinaryString( delay_timer) );

        soundTimerLED->setText( QString("0x%1").arg( sound_timer,0,16) );
        soundTimerLED->setToolTip( hexToBinaryString( sound_timer) );


        stackTEdit->clear();
        foreach (const quint16 m_val, m_stack) {
            stackTEdit->append( QString("0x%1").arg( m_val,0,16));
        }
    }
}

