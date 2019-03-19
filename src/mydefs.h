#ifndef MYDEFS_H
#define MYDEFS_H

#define DEBUG 1

#define RAM_SIZE 4096
#define START_ADDR 0x200

#define DISPLAY_X 64
#define DISPLAY_Y 32
#define MAX_DISPLAY_SIZE 64*32 - 1

#define LAPS_TYPE_1 10
#define LAPS_TYPE_2 30

#define KEY_PAD 16

#define REG_VF 15
#define MAX_REG 16

#define SMALL_FONT_SIZE 5
#define BIG_FONT_SIZE 10
#define BIG_FONT_OFFSET 80

#define REFRESH_TIME 1000 / 60

#ifdef DEBUG
#include <QTime>
#include <QDebug>
#endif

#include <QObject>

#endif // MYDEFS_H
