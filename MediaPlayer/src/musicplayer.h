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
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//����������
	QMediaPlayer *m_musicPlayer = nullptr;
	//�����б�
	QMediaPlaylist *m_musicPlayList = nullptr;
	//�б�ģ��
	QStandardItemModel *m_listItemModel = nullptr;
	//����Ŀ¼
	QString m_filePath;
	//��ʱ������
	QTimer *m_progressTimer;
	//����ʱ���ַ���
	QString m_musicTime;
	//����������
	VSliderWidget *m_volumeSlider = nullptr;
	//���ģ��
	QSharedPointer<LrcWidget> m_lrcWidget = nullptr;
};

#endif // MUSICPLAYER_H
