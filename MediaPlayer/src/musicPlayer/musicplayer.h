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
class VideoWidget;
class QMovie;

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
	void initInfo();
	void checkLrcWidget(int index);
	void parseJsonSongInfo(const QString& json);
	void parseJsonMvInfo(const QString& json);

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
	void sltSearchMV();
	void sltMouseDoubleClicked(int row);
	void sltNetWorkMusicPlay(QNetworkReply *reply);
	void sltDownLoadButtonClick();
	void sltDownLoadByNetWork(QNetworkReply *reply);
	void sltLookMv();
	void sltNetWorkMvInfo(QNetworkReply *reply);

signals:
	void sigReturnPanel();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::musicplayer ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//����������
	QMediaPlayer *m_musicPlayer = nullptr;
	//�����б�
	QMediaPlaylist *m_musicPlayList = nullptr;
	//����Ŀ¼
	QString m_filePath;
	//����ʱ���ַ���
	QString m_musicTime;
	//����������
	VSliderWidget *m_volumeSlider = nullptr;
	//���ģ��
	QSharedPointer<LrcWidget> m_lrcWidget = nullptr;
	//���������������
	QNetworkAccessManager *m_netWorkMusicPlay = nullptr;
	//��������
	QNetworkAccessManager *m_netWorkDownLoad = nullptr;
	//MV��ϸ��ȡ
	QNetworkAccessManager *m_netWorkMv = nullptr;
	//MV������
	QSharedPointer<VideoWidget> m_videoWidget = nullptr;
	//��̬����
	QMovie *m_bgMovie = nullptr;
};

#endif // MUSICPLAYER_H
