#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QPaintEvent>

class Display : public QWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = nullptr);
    void updateDisplay(QVector <unsigned char> screen);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    int getIndex(int x,int y);
    QImage* m_screenImage;
    QColor bgColor;
    QColor fgColor;
};

#endif // DISPLAY_H
