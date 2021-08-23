#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include "ui_videoPlayer.h"

class QMediaPlaylist;
class QMediaPlayer;
class QVideoWidget;
class QVBoxLayout;
class VideoControls;

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

private:
	void initVideoPlayModel();
	void initVideoControls();
	void locateWidgets();

private slots:
	void sltMaxOrNormal();
	void sltOpenLocalVideoList();
	void sltDurationChanged(qint64 duration);
	void sltPositionChanged(qint64 position);
	void sltListWidgetDoubleClicked(QListWidgetItem *item);
	void sltVideoPlayOrPause(bool state);
	void sltNextVideoPlay();
	void sltSetPlayCycle(bool state);
	void sltSoundVoiceValue(int value);
	void sltSetPosition(int value);
	void sltReturnPanel();

signals:
	void sigReturnPanel();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void moveEvent(QMoveEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool eventFilter(QObject *obj, QEvent *event);

	//键盘控制快进快退
	virtual void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::video ui;
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
	//视频控件
	QSharedPointer<VideoControls> m_videoControls = nullptr;
	//布局器
	QVBoxLayout* layout_video = nullptr;
	//视频时间字符串
	QString m_videoTime;
	//当前视频进度
	qint64 m_position;
	//声音初始值
	int m_volume;
};

#endif // MEDIAPLAYER_H
