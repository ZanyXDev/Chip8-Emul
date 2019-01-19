#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QStyle>

#include "mydefs.h"
#include "screenwidget.h"
#include "chip8emu.h"
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
    //void createActions();
    void setupFileActions();
    void setupGameActions();
    void setupOtherActions();
    void createStatusBar();
    void createGUI();
    void createConnection();

    quint8 mapKey(int mkey);

    bool workMode;
    QPoint calcDeskTopCenter(int width,int height);

    QAction* newGameAct;
    QRect desktopRect;

    QToolBar *tb;
    QTextEdit* textListing;
#ifdef DEBUG

    QLabel* CTime_label;
#endif
    QMenu* viewMenu;

    Chip8Emu *m_emul;
    ScreenWidget *m_screen;
    CPUBoxWidget *m_debugCPU;
    const QString rsrcPath = ":res/images/";

};

#endif // MAINWINDOW_H
