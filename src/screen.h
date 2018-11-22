#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QBitArray>
#include <QColor>
#include <QPainter>
#include <QBitArray>

#include "mydefs.h"


class Screen : public QWidget
{
    Q_OBJECT
public:
    explicit Screen(QWidget *parent = nullptr);
    QSize sizeHint() const;

signals:

public slots:
    void updateScreen(QBitArray display);

protected:
  void paintEvent(QPaintEvent *event);

private:
  QColor bgColor;
  QColor fgColor;
  QColor lineColor;
  QBitArray m_display;
  int zoom;

  void drawImagePixel(QPainter *painter, int x, int y);

};

#endif // SCREEN_H
