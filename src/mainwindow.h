#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QFileDialog>
#include <QByteArray>

#include "screen.h"
#include "chip8emu.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void startEmulation();
    void stopEmulation();
    void fileLoaded(QByteArray &data);

public slots:
    void startGame();
    void fileOpen();
    void readyToWork(bool flag);

private:
    void createActions();
    void createStatusBar();
    void createGUI();
    void createConnection();

    bool workMode;
    QPoint calcDeskTopCenter(int width,int height);

    QAction* newGameAct;
    QRect desktopRect;

    QComboBox* gameSelector;
    QPushButton* startGameBtn;
    QPushButton* nextStepBtn;
    QPushButton* stopGameBtn;

    QLabel* PC_label;
    QLabel* I_label;

    QLabel* V0_label;
    QLabel* V1_label;
    QLabel* V2_label;
    QLabel* V3_label;
    QLabel* V4_label;
    QLabel* V5_label;
    QLabel* V6_label;
    QLabel* V7_label;
    QLabel* V8_label;
    QLabel* V9_label;
    QLabel* Va_label;
    QLabel* Vb_label;
    QLabel* Vc_label;
    QLabel* Vd_label;
    QLabel* Ve_label;
    QLabel* Vf_label;

    Chip8Emu *m_emul;
    Screen* m_screen;
};

#endif // MAINWINDOW_H
