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

private:
    Ui::video ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//�б�
	QMediaPlaylist *m_videoPlayList = nullptr;
	//�б�洢ʵ��
	QVector<QString> m_listInfo;
	//����������
	QMediaPlayer* m_videoPlayer = nullptr;
	//���Ŵ���
	QVideoWidget* m_videoPlayWidget = nullptr;
	//��Ƶ�ؼ�
	QSharedPointer<VideoControls> m_videoControls = nullptr;
	//������
	QVBoxLayout* layout_video = nullptr;
	//��Ƶʱ���ַ���
	QString m_videoTime;
};

#endif // MEDIAPLAYER_H
