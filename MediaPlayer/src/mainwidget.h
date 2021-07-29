#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "ui_mainwidget.h"

class QMovie;
class QMediaPlayer;
class VideoPlayer;
class MusicPlayer;

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
	void sltOpenWeb(const QString &text);

signals:
    void MySig(); //返回上一级的信号

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWidget ui;
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//动态背景
    QMovie *m_bgMovie = nullptr;
	//背景音效
    QMediaPlayer *m_bgPlayer = nullptr;
	//视频播放器
	QSharedPointer<VideoPlayer> m_videoPlayer = nullptr;
	//音乐播放器
	QSharedPointer <MusicPlayer> m_musicPlayer = nullptr; 
	//更多
	QLabel *m_blogLabel = nullptr;
};

#endif // MAINWIDGET_H
