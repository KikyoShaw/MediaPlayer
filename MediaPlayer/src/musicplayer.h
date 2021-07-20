#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QSharedPointer>
#include "ui_musicplayer.h"

class QMediaPlayer;
class QMediaPlaylist;
class QStandardItemModel;
class VSliderWidget;
class LrcWidget;

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

private slots:
	void sltMaxOrNormal();
	void sltSliderProgressClicked();
	void sltSliderProgressMoved();
	void sltSliderProgressReleased();
	void sltMusicPlayOrPause();
	void sltNextMusicPlay();
	void sltPrevMusicPlay();
	void sltOpenLocalMusicList();
	void sltPlayListClicked(QModelIndex index);
	void sltTimerOut();
	void sltDurationChanged(qint64 duration);
	void sltPositionChanged(qint64 position);
	void sltSoundVoiceValue(int value);
	void sltShowVolumeSlider();
	void sltSetPlayCycle();
	void sltShowLrcModel();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::musicplayer ui;
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//播放器对象
	QMediaPlayer *m_musicPlayer = nullptr;
	//歌曲列表
	QMediaPlaylist *m_musicPlayList = nullptr;
	//列表模块
	QStandardItemModel *m_listItemModel = nullptr;
	//所在目录
	QString m_filePath;
	//定时器工作
	QTimer *m_progressTimer;
	//歌曲时间字符串
	QString m_musicTime;
	//声音进度条
	VSliderWidget *m_volumeSlider = nullptr;
	//歌词模块
	QSharedPointer<LrcWidget> m_lrcWidget = nullptr;
};

#endif // MUSICPLAYER_H
