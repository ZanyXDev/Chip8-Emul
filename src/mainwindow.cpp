#include "mainwindow.h"

#define DEBUG 1

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QDesktopWidget desktop;
    desktopRect = desktop.availableGeometry(desktop.primaryScreen());

    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->setMaximumSize(desktopRect.width() - 5, desktopRect.height() - 5);
    // Move app window to center desktop
    this->move(calcDeskTopCenter(this->width(),this->height()));

    createActions();
    createGUI();
    createStatusBar();
    createConnection();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->onStopClicked();
    QMainWindow::closeEvent(event);
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
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    QByteArray tmp;

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    tmp = file.readAll();

    if (tmp.isEmpty()){
        return;
    }

    m_emul.setMemory( tmp );
    this->readyToWork( true );
    gameSelector->addItem(fileName);
}

void MainWindow::readyToWork(bool flag)
{
    newGameAct->setEnabled( flag );
    startGameBtn->setEnabled( flag );
    nextStepBtn->setEnabled( flag );
    stopGameBtn->setEnabled( flag );
}

void MainWindow::onStartClicked()
{
    this->statusBar()->showMessage(tr("Start CPU emulation"));
    // Запуск потока
    m_emul.setRunning(true);
    cpuThread.start();
}

void MainWindow::onStopClicked()
{
    this->statusBar()->showMessage(tr("Stop CPU emulation"),2000);
    this->readyToWork( false );
    // Остановка потока через завершение выполнения методов run в объектах
    m_emul.setRunning(false);
}

// -------------------------------------------------------------------------------------------
void MainWindow::createActions()
{    
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *fileLoadAct = fileMenu->addAction(tr("&Load ROM"), this, &MainWindow::fileOpen);
    fileLoadAct->setShortcuts(QKeySequence::Open);
    fileLoadAct->setStatusTip(tr("Load ROM from file..."));

    fileMenu->addSeparator();

    QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));

    menuBar()->addSeparator();
    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));

    newGameAct = new QAction(tr("&Start game"),this);
    newGameAct->setStatusTip(tr("Start new game"));
    newGameAct->setEnabled( false );
    gameMenu->addAction(newGameAct);
    menuBar()->addSeparator();

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
    QFrame *frame = new QFrame;
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    QHBoxLayout *cmdLayout = new QHBoxLayout();

    gameSelector = new QComboBox();
#ifdef DEBUG
    gameSelector->addItem(tr("Game 1"));
    gameSelector->addItem(tr("Game 2"));
    gameSelector->addItem(tr("Game 3 with Long Name"));
#endif
    cmdLayout->addWidget(gameSelector);
    cmdLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Expanding));

    startGameBtn = new QPushButton("&Start");
    startGameBtn->setEnabled( false);
    cmdLayout->addWidget(startGameBtn);

    nextStepBtn = new QPushButton("&Next step");
    nextStepBtn->setEnabled( false );
    cmdLayout->addWidget(nextStepBtn);

    stopGameBtn = new QPushButton("&Stop");
    stopGameBtn->setEnabled( false );
    cmdLayout->addWidget(stopGameBtn);

    QHBoxLayout *mainLayout = new QHBoxLayout();

    m_screen = new Screen();
    mainLayout->addWidget(m_screen);

    QTextEdit* textListing = new QTextEdit();
    textListing->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); // set ideal size as minimum Qimage size/ Can expand, only
    textListing->setReadOnly(true);

#ifdef DEBUG
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
    textListing->append(tr("0x200   2A0F    CLS"));
    textListing->append(tr("0x201   1A0F    JP 0x800"));
#endif
    mainLayout->addWidget(textListing);


    QHBoxLayout *countersLayout = new QHBoxLayout();
    PC_label = new QLabel("PC count:0x200");
    I_label  = new QLabel("I  count:0x000");
    countersLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Expanding));
    countersLayout->addWidget(PC_label);
    countersLayout->addWidget(I_label);

    frameLayout->addLayout(cmdLayout);
    frameLayout->addLayout(mainLayout);
    frameLayout->addLayout(countersLayout);
    //    frameLayout->addLayout(cpuLayout);
    frameLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Expanding));
    setCentralWidget(frame);
}

void MainWindow::createConnection()
{
    connect(newGameAct, &QAction::triggered, this, &MainWindow::startGame);
    connect(startGameBtn,&QPushButton::clicked,this,&MainWindow::onStartClicked);
    connect(stopGameBtn,&QPushButton::clicked,this,&MainWindow::onStopClicked);

    // Запуск выполнения метода run будет осуществляться по сигналу запуска от соответствующего потока
    connect(&cpuThread, &QThread::started, &m_emul, &CPUThread::run);

    // Остановка потока же будет выполняться по сигналу finished от соответствующего объекта в потоке
    connect(&m_emul, &CPUThread::finished, &cpuThread, &QThread::terminate);
    connect(&m_emul, &CPUThread::screenChanged,m_screen,&Screen::updateScreen);

    m_emul.moveToThread(&cpuThread);    // Передаём объекты в соответствующие потоки
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


