#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H
#include <QSlider>
#include <QMouseEvent>
#include <QCoreApplication>

class CustomSlider : public QSlider
{
    Q_OBJECT
public:
    CustomSlider(QWidget *parent = 0) : QSlider(parent)
    {
    }
protected:
    void mousePressEvent(QMouseEvent *ev);//重写QSlider的mousePressEvent事件
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void costomSliderClicked();//自定义的鼠标单击信号，用于捕获并处理
    void costomSliderRelese();

public:
    bool status = false;

};

#endif // CUSTOMSLIDER_H
