#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QPaintEvent>

class Display : public QWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void setAntialiased(bool antialiased);
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void debugDraw();

    QImage m_screenImage;
    QColor bgColor;
    QColor fgColor;
    bool antialiased;
};

#endif // DISPLAY_H
