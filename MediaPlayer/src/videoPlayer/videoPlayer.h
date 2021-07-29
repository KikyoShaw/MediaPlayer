#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include "ui_videoPlayer.h"
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include "customslider.h"

class QMediaPlaylist;
class QMediaPlayer;
class QVideoWidget;
class QVBoxLayout;

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    //切换视频标志位
    static int play_flag;

    bool status = false;

private:
	void initVideoPlayModel();

private slots:
	void sltMaxOrNormal();
	void sltOpenLocalVideoList();
	void sltDurationChanged(qint64 duration);
	void sltPositionChanged(qint64 position);
	void sltListWidgetDoubleClicked(QListWidgetItem *item);

    //void on_pushButton_open_file_clicked();
    //void on_pushButton_play_and_pause_clicked();
    //void onTimerOut();
    //void slider_progress_clicked();
    //void slider_released();
    //void slider_progress_moved();
    //void slider_progress_released();
    //void on_pushButton_volume_clicked();
    //void slider_volume_changed();
    //void on_pushButton_flont_clicked();

    //void on_pushButton_live_clicked();

signals:
    void mySignal();


private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::video ui;
    int liveStatus=0;//在线状态
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//列表
	QMediaPlaylist *m_videoPlayList = nullptr;
	//列表存储实体
	QVector<QString> m_listInfo;
	//播放器对象
	QMediaPlayer* m_videoPlayer = nullptr;
	//播放窗口
	QVideoWidget* m_videoPlayWidget = nullptr;
	//布局器
	QVBoxLayout* layout_video = nullptr;
	//视频时间字符串
	QString m_videoTime;
};

#endif // MEDIAPLAYER_H
