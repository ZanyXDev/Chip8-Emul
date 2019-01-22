#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QDesktopWidget desktop;
    desktopRect = desktop.availableGeometry(desktop.primaryScreen());

    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    this->setMaximumSize(desktopRect.width() - 5, desktopRect.height() - 5);
    // Move app window to center desktop
    this->move(calcDeskTopCenter(this->width(),this->height()));
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    m_display = new Display();
    m_keyboard = new Keyboard();

    m_emul = new Chip8Emu();
    m_emul->setDisplay( m_display );
    m_emul->setKeyboard( m_keyboard );

    setupFileActions();
    setupGameActions();
    setupOtherActions();
    createGUI();
    createStatusBar();
    createConnection();

}

// -------------------------------------- PUBLIC SLOTS ---------------------------------------
void MainWindow::startGame()
{

}

void MainWindow::fileOpen()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select chip-8 game file"),
                                                    "",
                                                    tr("Chip-8 game files (*.ch8)"),
                                                    &selectedFilter);
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);


    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray tmp = file.readAll();

    if (tmp.isEmpty())
    {
        return;
    }

    emit fileLoaded( tmp );
    //gameSelector->addItem(fileName);
}

void MainWindow::readyToWork(bool flag)
{
    //    newGameAct->setEnabled( flag );
    //    startGameBtn->setEnabled( flag );
    //    nextStepBtn->setEnabled( flag );
    //    stopGameBtn->setEnabled( flag );
}

void MainWindow::setupFileActions()
{
    QToolBar *tb = addToolBar(tr("File Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&File"));

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png"));
    QAction *a = menu->addAction(openIcon, tr("&Open..."), this, &MainWindow::fileOpen);
    a->setShortcut(QKeySequence::Open);
    tb->addAction(a);
    menu->addSeparator();

    a = menu->addAction(tr("&Quit"), this, &QWidget::close);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
}

void MainWindow::setupGameActions()
{
    QToolBar *tb = addToolBar(tr("Game Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&Game"));

    QAction *actionStart = menu->addAction(style()->standardIcon(QStyle::SP_MediaPlay),
                                           tr("&Start..."), m_emul,&Chip8Emu::startEmulation);
    tb->addAction( actionStart );
    menu->addAction( actionStart );

    QAction *actionStep = menu->addAction(style()->standardIcon(QStyle::SP_MediaSkipForward),
                                          tr("&Start..."), m_emul,&Chip8Emu::stepEmulation);
    tb->addAction( actionStep );
    menu->addAction( actionStep );

    QAction *actionStop = menu->addAction(style()->standardIcon(QStyle::SP_MediaStop),
                                          tr("&Stop..."), m_emul,&Chip8Emu::stopEmulation);
    tb->addAction( actionStop );
    menu->addAction( actionStop );

}

void MainWindow::setupOtherActions()
{
    viewMenu = menuBar()->addMenu(tr("&View"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    //QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    //aboutAct->setStatusTip(tr("Show the application's About box"));
    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::createStatusBar()
{
    this->statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createGUI()
{
    QDockWidget *dock = new QDockWidget(tr("Main chip-8 window"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_screen = new ScreenWidget();
    m_screen->setDisplay( m_display );
    dock->setWidget( m_screen );
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Debug CPU window"), this);
    m_debugCPU = new CPUBoxWidget();
    m_debugCPU->setSizePolicy(QSizePolicy::Minimum,
                              QSizePolicy::Minimum);

    dock->setWidget( m_debugCPU );
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("ASM listing window"), this);
    textListing = new QTextEdit();
    textListing->setReadOnly(true);

    dock->setWidget( textListing );
    dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::createConnection()
{
    /*
    connect(newGameAct, &QAction::triggered, this, &MainWindow::startGame);
    connect(m_emul,&Chip8Emu::ReadyToWork,this,&MainWindow::readyToWork);
    connect(m_emul,&Chip8Emu::showTime,this,&);
*/
    connect(this,&MainWindow::fileLoaded,m_emul,&Chip8Emu::loadData2Memory);    
    connect(this,&MainWindow::changeKeyState,m_keyboard,&Keyboard::changeKeyState);

    connect(m_emul,&Chip8Emu::updateScreen,m_screen,&ScreenWidget::updateScreen);
    connect(m_emul,&Chip8Emu::showDecodeOpCode,textListing,&QTextEdit::append);

    connect(m_emul,&Chip8Emu::pointerCodeChanged,  m_debugCPU,&CPUBoxWidget::pointerCodeChanged);
    connect(m_emul,&Chip8Emu::registerIChanged,    m_debugCPU,&CPUBoxWidget::registerIChanged);
    connect(m_emul,&Chip8Emu::delayTimerChanged,   m_debugCPU,&CPUBoxWidget::delayTimerChanged);
    connect(m_emul,&Chip8Emu::soundTimerChanged,   m_debugCPU,&CPUBoxWidget::soundTimerChanged);
    connect(m_emul,&Chip8Emu::registerValueChanged,m_debugCPU,&CPUBoxWidget::registerValueChanged);

    connect(m_emul,&Chip8Emu::stackPop, m_debugCPU,&CPUBoxWidget::stackPop);
    connect(m_emul,&Chip8Emu::stackPush,m_debugCPU,&CPUBoxWidget::stackPush);
}

quint8 MainWindow::mapKey(int mkey)
{
    /**  Real      Emu
  * +-+-+-+-+  | +-+-+-+-+
  * |1|2|3|C|  | |1|2|3|4|
  * +-+-+-+-+  | +-+-+-+-+
  * |4|5|6|D|  | |Q|W|E|R|
  * +-+-+-+-+  | +-+-+-+-+
  * |7|8|9|E|  | |A|S|D|F|
  * +-+-+-+-+  | +-+-+-+-+
  * |A|0|B|F|  | |Z|X|C|V|
  * +-+-+-+-+  | +-+-+-+-+
  **/
    quint8 m_key = '\0';
    switch ( mkey )
    {
    case Qt::Key_1: m_key = 1;  break;
    case Qt::Key_2: m_key = 2;  break;
    case Qt::Key_3: m_key = 3;  break;
    case Qt::Key_4: m_key = 12;  break;
    case Qt::Key_Q: m_key = 4;  break;
    case Qt::Key_W: m_key = 5;  break;
    case Qt::Key_E: m_key = 6;  break;
    case Qt::Key_R: m_key = 13;  break;
    case Qt::Key_A: m_key = 7;  break;
    case Qt::Key_S: m_key = 8;  break;
    case Qt::Key_D: m_key = 9; break;
    case Qt::Key_F: m_key = 15; break;
    case Qt::Key_Z: m_key = 10; break;
    case Qt::Key_X: m_key = 0; break;
    case Qt::Key_C: m_key = 11; break;
    case Qt::Key_V: m_key = 15; break;
    default: break;
    }
    return m_key;
}

QPoint MainWindow::calcDeskTopCenter(int width,int height)
{
    QPoint centerWindow;
    //получаем прямоугольник с размерами как у экрана
    centerWindow = desktopRect.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (width / 2) );
    centerWindow.setY(centerWindow.y() - (height / 2) );
    return centerWindow;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //TODO use QMutex for lock Key state and save key state to QSet
    emit changeKeyState( mapKey( event->key() ),true );
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    emit changeKeyState( mapKey( event->key() ), false );
    QMainWindow::keyReleaseEvent(event);
}





