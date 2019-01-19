#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QBitArray>
#include <QColor>
#include <QPainter>
#include <QBitArray>
#include <QPaintEvent>
#include <QPixmap>

#include "mydefs.h"
#include "display.h"

class ScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenWidget(QWidget *parent = nullptr);
    QSize sizeHint() const;
    void setDisplay( Display *display );
signals:

public slots:
    void updateScreen(QBitArray display);

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPixmap* m_pixmap;
  QColor bgColor;
  QColor fgColor;
  QColor lineColor;
  QBitArray m_display_old;
  int zoom;
  Display *m_display;


  void drawImagePixel(QPainter *painter, int x, int y);

  void drawOnSurface();

};

#endif // SCREENWIDGET_H
