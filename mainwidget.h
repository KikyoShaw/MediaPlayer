#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMovie>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMediaPlayer>
#include <QMessageBox>

#include "mediaplayer.h"
#include "musicplayer.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void on_vedio_clicked();
    void on_music_clicked();
    void on_exit_clicked();
    void on_help_clicked();
    void my_player();

signals:
    void MySig(); //返回上一级的信号

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWidget *ui;
    QPoint point;
    QMovie *movie1;
    QMediaPlayer *player;
    MediaPlayer *media; //视频播放器
    musicplayer *music; //音乐播放器
};

#endif // MAINWIDGET_H
