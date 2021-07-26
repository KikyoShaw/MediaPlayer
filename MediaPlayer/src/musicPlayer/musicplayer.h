#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QSharedPointer>
#include "ui_musicplayer.h"

class QMediaPlayer;
class QMediaPlaylist;
class VSliderWidget;
class LrcWidget;
class QNetworkAccessManager;
class QNetworkReply;

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

private:
	void initMusicPlayer();
	void initLrcModel();
	void initVolumeSlider();
	void initWebModel();
	void checkLrcWidget(int index);
	void parseJsonSongInfo(const QString& json);

private slots:
	void sltMaxOrNormal();
	void sltSliderProgressClicked();
	void sltSliderProgressReleased();
	void sltMusicPlayOrPause();
	void sltNextMusicPlay();
	void sltPrevMusicPlay();
	void sltOpenLocalMusicList();
	void sltPlayListClicked(int row);
	void sltDurationChanged(qint64 duration);
	void sltPositionChanged(qint64 position);
	void sltSoundVoiceValue(int value);
	void sltShowVolumeSlider();
	void sltSetPlayCycle();
	void sltShowLrcModel();
	void sltReturnPanel();
	void sltSearchMusic();
	void sltMouseDoubleClicked(int row);
	void sltNetWorkMusicPlay(QNetworkReply *reply);
	void sltDownLoadButtonClick();
	void sltDownLoadByNetWork(QNetworkReply *reply);

signals:
	void sigReturnPanel();

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
	//所在目录
	QString m_filePath;
	//歌曲时间字符串
	QString m_musicTime;
	//声音进度条
	VSliderWidget *m_volumeSlider = nullptr;
	//歌词模块
	QSharedPointer<LrcWidget> m_lrcWidget = nullptr;
	//网络歌曲播放请求
	QNetworkAccessManager *m_netWorkMusicPlay = nullptr;
	//下载请求
	QNetworkAccessManager *m_netWorkDownLoad = nullptr;
};

#endif // MUSICPLAYER_H
