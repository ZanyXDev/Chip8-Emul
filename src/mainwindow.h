#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QDockWidget>

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>

#include <QFileDialog>
#include <QByteArray>
#include <QKeyEvent>

#include "screen.h"
#include "chip8emu.h"
#include "mydefs.h"
#include "cpuboxwidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void startEmulation();
    void stopEmulation();
    void fileLoaded(QByteArray &data);
    void changeKeyState(quint8 key, bool state);

public slots:
    void startGame();
    void fileOpen();
    void readyToWork(bool flag);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    void createActions();
    void createStatusBar();
    void createGUI();
    void createConnection();

    quint8 mapKey(int mkey);

    bool workMode;
    QPoint calcDeskTopCenter(int width,int height);

    QAction* newGameAct;
    QRect desktopRect;

//    QComboBox* gameSelector;
//    QPushButton* startGameBtn;
//    QPushButton* nextStepBtn;
//    QPushButton* stopGameBtn;

//    QLabel* PC_label;
//    QLabel* I_label;

#ifdef DEBUG
    QTextEdit* textListing;
    QLabel* CTime_label;
#endif
    QMenu* viewMenu;

    Chip8Emu *m_emul;
    Screen* m_screen;
    CPUBoxWidget *m_debugCPU;
};

#endif // MAINWINDOW_H
