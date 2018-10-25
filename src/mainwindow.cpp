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
}

void MainWindow::createActions()
{    
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();

    QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));

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
    cmdLayout->addWidget(startGameBtn);

    nextStepBtn = new QPushButton("&Next step");
    nextStepBtn->addAction();
    cmdLayout->addWidget(nextStepBtn);

    QHBoxLayout *mainLayout = new QHBoxLayout();

    Screen* m_screen = new Screen();
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

QPoint MainWindow::calcDeskTopCenter(int width,int height)
{
    QPoint centerWindow;
    //получаем прямоугольник с размерами как у экрана
    centerWindow = desktopRect.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (width / 2) );
    centerWindow.setY(centerWindow.y() - (height / 2) );
    return centerWindow;
}


