#ifndef START_H
#define START_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QHBoxLayout>
#include <QMediaPlayer>
#include <QTimer>
#include <QDebug>
#include <QTime>
#include <QPoint>
#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>

namespace Ui {
class Start;
}

class Start : public QWidget
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();
    int num;
    QPoint point;
    QMediaPlayer *Myplayer;

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    Ui::Start *ui;
    QVideoWidget *video;
};

#endif // START_H
